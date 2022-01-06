#include <employ_files.h>
#include <employ_database.h>
#include <employ_notify.h>
#include <wsjcpp_hashes.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <sys/stat.h>


REGISTRY_WJSCPP_EMPLOY(EmployFiles)

// ---------------------------------------------------------------------

EmployFiles::EmployFiles()
    : WsjcppEmployBase(EmployFiles::name(), { EmployDatabase::name(), EmployGlobalSettings::name(), EmployNotify::name() }) {
    TAG = EmployFiles::name();
}

// ---------------------------------------------------------------------

bool EmployFiles::init() {
    std::lock_guard<std::mutex> lock(m_mtxFiles);
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
        auto pModelQuestFile = new ModelQuestFile();
        pModelQuestFile->setLocalId(record.value("id").toInt());
        std::string sUuid = record.value("uuid").toString().toStdString();
        pModelQuestFile->setUuid(sUuid);
        pModelQuestFile->setQuestLocalId(record.value("questid").toInt());
        int nDownloadsCounter = record.value("downloads_counter").toInt();
        pModelQuestFile->setDownloadsCounter(nDownloadsCounter);
        m_nAllDownloadsCounter += nDownloadsCounter;
        pModelQuestFile->setFileName(record.value("filename").toString().toStdString());
        int nFileSize = record.value("size").toString().toInt();
        // TODO check max size
        m_nSizeOfFiles += nFileSize;
        pModelQuestFile->setFileSize(nFileSize);
        pModelQuestFile->setDatetime(record.value("dt").toString().toStdString());
        std::string sFilePath = record.value("filepath").toString().toStdString();
        sFilePath = WsjcppCore::doNormalizePath("/" + sFilePath);
        pModelQuestFile->setFilePath(sFilePath);
        pModelQuestFile->setMd5(record.value("md5").toString().toStdString());
        // TODO must be cached all fields

        if (m_mapCacheQuestFilesByUuid.count(sUuid)) {
            WsjcppLog::err(TAG, "Inconsistent list quests in database by uuid: " + sUuid);
            return false;
        }
        m_mapCacheQuestFilesByUuid.insert(std::pair<std::string, ModelQuestFile*>(sUuid, pModelQuestFile));

        if (m_mapCacheQuestFilesByFilePath.count(sFilePath)) {
            WsjcppLog::err(TAG, "Inconsistent list quests in database by filepath: " + sFilePath);
            return false;
        }
        WsjcppLog::info(TAG, "Registering... file " + sFilePath);
        m_mapCacheQuestFilesByFilePath.insert(std::pair<std::string, ModelQuestFile*>(sFilePath, pModelQuestFile));

        // check filepath
        std::string sFullPath = sPublicFolder + "/" + sFilePath;
        if (!WsjcppCore::fileExists(sFullPath)) {
            WsjcppLog::err(TAG, "Not found file by path: " + sFullPath + "\r\n Please try fix it and try again start server");
            return false;
        }

        // fix md5 for file
        if (pModelQuestFile->getMd5() == "") {
            std::string sHash = WsjcppHashes::getMd5ByFile(sFullPath);
            WsjcppLog::info(TAG, "Hash: " + sHash);
            pModelQuestFile->setMd5(sHash);

            EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
            QSqlDatabase db = *(pDatabase->database());
            {
                QSqlQuery query(db);
                query.prepare("UPDATE quests_files SET md5 = :md5 WHERE uuid = :uuid");
                query.bindValue(":md5", QString::fromStdString(sHash));
                query.bindValue(":uuid", QString::fromStdString(pModelQuestFile->getUuid()));
                if (!query.exec()) {
                    WsjcppLog::err(TAG, query.lastError().text().toStdString());
                }
            }
        }

        m_vCacheQuestFiles.push_back(pModelQuestFile);
    }

    return true;
}

bool EmployFiles::deinit() {
    // TODO
    return true;
}

int EmployFiles::getCountOfFiles() {
    return m_vCacheQuestFiles.size();
}

int EmployFiles::getSizeOfFiles() {
    return m_nSizeOfFiles;
}

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

std::vector<ModelQuestFile*> EmployFiles::findFilesByQuestId(int nQuestId) {
    std::lock_guard<std::mutex> lock(m_mtxFiles);
    std::vector<ModelQuestFile*> vRet;
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
        ModelQuestFile* pModel = m_mapCacheQuestFilesByUuid.at(m.getUuid());
        int nNewVal = pModel->getDownloadsCounter() + 1;
        m_nAllDownloadsCounter++;
        pModel->setDownloadsCounter(nNewVal);

        EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
        QSqlDatabase db = *(pDatabase->database());
        {
            QSqlQuery query(db);
            query.prepare("UPDATE quests_files SET downloads_counter = :downloads_counter WHERE uuid = :uuid");
            query.bindValue(":downloads_counter", nNewVal);
            query.bindValue(":uuid", QString::fromStdString(pModel->getUuid()));
            if (!query.exec()) {
                WsjcppLog::err(TAG, query.lastError().text().toStdString());
                return;
            }
        }
    }
}