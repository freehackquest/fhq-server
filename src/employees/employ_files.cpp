/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <employ_database.h>
#include <employ_files.h>
#include <employ_notify.h>
#include <sys/stat.h>
#include <wsjcpp_hashes.h>

REGISTRY_WJSCPP_EMPLOY(EmployFiles)

// ---------------------------------------------------------------------

EmployFiles::EmployFiles()
    : WsjcppEmployBase(EmployFiles::name(),
                       {EmployDatabase::name(), EmployGlobalSettings::name(), EmployNotify::name()}) {
  TAG = EmployFiles::name();
}

// ---------------------------------------------------------------------

bool EmployFiles::init() {
  // std::lock_guard<std::mutex> lock(m_mtxFiles);
  // check the access to games folder
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  std::string sPublicFolder = pGlobalSettings->get("web_public_folder").getDirPathValue();
  std::string sBasePath = WsjcppCore::doNormalizePath(sPublicFolder + "/quests");
  if (!WsjcppCore::dirExists(sBasePath)) {
    WsjcppCore::makeDir(sBasePath);
  }
  std::string targetTestFile = WsjcppCore::doNormalizePath(sBasePath + "/test");

  // test create file
  if (!WsjcppCore::createEmptyFile(targetTestFile)) {
    WsjcppLog::err(TAG, "Cannot access to write " + targetTestFile);
    return false;
  }
  if (!WsjcppCore::removeFile(targetTestFile)) {
    WsjcppLog::err(TAG, "Could not delete file " + targetTestFile);
    return false;
  }

  // load list of games to cache
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  m_nSizeOfFiles = 0;
  m_nAllDownloadsCounter = 0;
  QSqlQuery query(db);
  query.prepare("SELECT * FROM quests_files");

  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
    return false;
  }

  while (query.next()) {
    QSqlRecord record = query.record();
    ModelQuestFile file;
    file.setLocalId(record.value("id").toInt());
    std::string sUuid = record.value("uuid").toString().toStdString();
    file.setUuid(sUuid);
    file.setQuestLocalId(record.value("questid").toInt());
    int nDownloadsCounter = record.value("downloads_counter").toInt();
    file.setDownloadsCounter(nDownloadsCounter);
    file.setFileName(record.value("filename").toString().toStdString());
    int nFileSize = record.value("size").toString().toInt();
    // TODO check max size

    file.setFileSize(nFileSize);
    file.setDatetime(record.value("dt").toString().toStdString());
    std::string sFilePath = record.value("filepath").toString().toStdString();
    sFilePath = WsjcppCore::doNormalizePath("/" + sFilePath);
    file.setFilePath(sFilePath);
    file.setMd5(record.value("md5").toString().toStdString());
    // TODO must be cached all fields

    // check filepath
    std::string sFullPath = sPublicFolder + "/" + sFilePath;
    if (!WsjcppCore::fileExists(sFullPath)) {
      WsjcppLog::err(TAG, "Not found file by path: " + sFullPath + "\r\n Please try fix it and try again start server");
      return false;
    }

    // ad-hoc: fix md5 for file
    if (file.getMd5() == "") {
      std::string sHash = WsjcppHashes::getMd5ByFile(sFullPath);
      WsjcppLog::info(TAG, "Hash: " + sHash);
      file.setMd5(sHash);

      EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
      QSqlDatabase db = *(pDatabase->database());
      {
        QSqlQuery query(db);
        query.prepare("UPDATE quests_files SET md5 = :md5 WHERE uuid = :uuid");
        query.bindValue(":md5", QString::fromStdString(sHash));
        query.bindValue(":uuid", QString::fromStdString(file.getUuid()));
        if (!query.exec()) {
          WsjcppLog::err(TAG, query.lastError().text().toStdString());
        }
      }
    }
    std::string sError;
    if (!registerFile(file, sError)) {
      WsjcppLog::err(TAG, sError);
      return false;
    }
  }

  return true;
}

bool EmployFiles::deinit() {
  // TODO
  return true;
}

int EmployFiles::getCountOfFiles() { return m_vCacheQuestFiles.size(); }

int EmployFiles::getSizeOfFiles() { return m_nSizeOfFiles; }

int EmployFiles::getAllDownloadsCounter() { return m_nAllDownloadsCounter; }

bool EmployFiles::findQuestFile(int nLocalId, ModelQuestFile &modelGame) {
  std::lock_guard<std::mutex> lock(m_mtxFiles);
  for (int i = 0; i < m_vCacheQuestFiles.size(); i++) { // TODO create map with index
    if (m_vCacheQuestFiles[i]->getLocalId() == nLocalId) {
      modelGame.copy(*m_vCacheQuestFiles[i]);
      return true;
    }
  }
  return false;
}

bool EmployFiles::findQuestFileByFilePath(const std::string &sFilePath, ModelQuestFile &m) {
  std::lock_guard<std::mutex> lock(m_mtxFiles);
  if (!m_mapCacheQuestFilesByFilePath.count(sFilePath)) {
    return false;
  }
  m.copy(*m_mapCacheQuestFilesByFilePath.at(sFilePath));
  return true;
}

bool EmployFiles::findQuestFileByUuid(const std::string &sUuid, ModelQuestFile &m) {
  std::lock_guard<std::mutex> lock(m_mtxFiles);
  if (!m_mapCacheQuestFilesByUuid.count(sUuid)) {
    return false;
  }
  m.copy(*m_mapCacheQuestFilesByUuid.at(sUuid));
  return true;
}

std::vector<ModelQuestFile *> EmployFiles::findFilesByQuestId(int nQuestId) {
  std::lock_guard<std::mutex> lock(m_mtxFiles);
  std::vector<ModelQuestFile *> vRet;
  for (int i = 0; i < m_vCacheQuestFiles.size(); i++) { // TODO create map with index
    if (m_vCacheQuestFiles[i]->getQuestLocalId() == nQuestId) {
      vRet.push_back(m_vCacheQuestFiles[i]);
    }
  }
  return vRet;
}

void EmployFiles::updateDownloadsCounter(const ModelQuestFile &m) {
  std::lock_guard<std::mutex> lock(m_mtxFiles);
  if (m_mapCacheQuestFilesByUuid.count(m.getUuid())) {
    ModelQuestFile *pModel = m_mapCacheQuestFilesByUuid.at(m.getUuid());
    int nNewVal = pModel->getDownloadsCounter() + 1;
    m_nAllDownloadsCounter++;
    pModel->setDownloadsCounter(nNewVal);

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    {
      QSqlQuery query(db);
      query.prepare("UPDATE quests_files SET downloads_counter = "
                    ":downloads_counter WHERE uuid = :uuid");
      query.bindValue(":downloads_counter", nNewVal);
      query.bindValue(":uuid", QString::fromStdString(pModel->getUuid()));
      if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
        return;
      }
    }
  }
}

bool EmployFiles::addFile(ModelQuestFile &m, std::string &sError) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  {
    QSqlQuery query(db);
    query.prepare("INSERT INTO quests_files(uuid, questid, filename, size, dt, filepath, "
                  "md5) "
                  "VALUES(:uuid, :questid, :filename, :size, NOW(), :filepath, :md5)");

    query.bindValue(":uuid", QString::fromStdString(m.getUuid()));
    query.bindValue(":questid", m.getQuestLocalId());
    query.bindValue(":filename", QString::fromStdString(m.getFileName()));
    query.bindValue(":size", m.getFileSize());
    query.bindValue(":filepath", QString::fromStdString(m.getFilePath()));
    query.bindValue(":md5", QString::fromStdString(m.getMd5()));

    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return false;
    }
    int nFileLocalId = query.lastInsertId().toInt();
    m.setLocalId(nFileLocalId);
  }
  return registerFile(m, sError);
}

bool EmployFiles::removeFileById(int nFileLocalId, std::string &sError) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  {
    QSqlQuery query(db);
    query.prepare("DELETE FROM quests_files WHERE id = :fileid");
    query.bindValue(":fileid", nFileLocalId);
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return false;
    }
  }

  ModelQuestFile file;
  bool bFound = false;
  for (int i = 0; i < m_vCacheQuestFiles.size(); i++) { // TODO create map with index
    if (m_vCacheQuestFiles[i]->getLocalId() == nFileLocalId) {
      file.copy(*m_vCacheQuestFiles[i]);
      bFound = true;
    }
  }
  if (!bFound) {
    sError = "Not found file";
    return false;
  }

  return unregisterFile(file, sError);
}

bool EmployFiles::registerFile(const ModelQuestFile &m, std::string &sError) {
  std::lock_guard<std::mutex> lock(m_mtxFiles);
  m_nSizeOfFiles += m.getFileSize();
  m_nAllDownloadsCounter += m.getDownloadsCounter();

  auto pFile = new ModelQuestFile();
  pFile->copy(m);

  if (m_mapCacheQuestFilesByUuid.count(pFile->getUuid())) {
    sError = "Inconsistent list quests in database by uuid: " + pFile->getUuid();
    return false;
  }
  m_mapCacheQuestFilesByUuid.insert(std::pair<std::string, ModelQuestFile *>(pFile->getUuid(), pFile));

  if (m_mapCacheQuestFilesByFilePath.count(pFile->getFilePath())) {
    sError = "Inconsistent list quests in database by filepath: " + pFile->getFilePath();
    return false;
  }
  WsjcppLog::info(TAG, "Registering... file " + pFile->getFilePath());
  m_mapCacheQuestFilesByFilePath.insert(std::pair<std::string, ModelQuestFile *>(pFile->getFilePath(), pFile));

  m_vCacheQuestFiles.push_back(pFile);
  return true;
}

bool EmployFiles::unregisterFile(const ModelQuestFile &m, std::string &sError) {
  std::lock_guard<std::mutex> lock(m_mtxFiles);
  m_nSizeOfFiles -= m.getFileSize();
  m_nAllDownloadsCounter -= m.getDownloadsCounter();

  m_mapCacheQuestFilesByUuid.erase(m.getUuid());
  m_mapCacheQuestFilesByFilePath.erase(m.getFilePath());

  int nIndex = -1;
  for (int i = 0; i < m_vCacheQuestFiles.size(); i++) { // TODO create map with index
    if (m_vCacheQuestFiles[i]->getLocalId() == m.getLocalId()) {
      nIndex = i;
      break;
    }
  }
  if (nIndex < 0) {
    sError = "Not found file";
    return false;
  }
  ModelQuestFile *pFile = m_vCacheQuestFiles[nIndex];
  m_vCacheQuestFiles.erase(m_vCacheQuestFiles.begin() + nIndex);
  delete pFile;
  return true;
}