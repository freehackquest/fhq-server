/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2025 FreeHackQuest <freehackquest@gmail.com>
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

#include "fhq_server_database_file.h"

#include <employees.h>
#include <sqlite3.h>

// ---------------------------------------------------------------------
// FhqServerDatabaseFileUpdateInfo

FhqServerDatabaseFileUpdateInfo::FhqServerDatabaseFileUpdateInfo(
  const std::string &sVersionFrom, const std::string &sVersionTo, const std::string &sDescription
)
  : m_sVersionFrom(sVersionFrom), m_sVersionTo(sVersionTo), m_sDescription(sDescription) {}

const std::string &FhqServerDatabaseFileUpdateInfo::versionFrom() const { return m_sVersionFrom; }

const std::string &FhqServerDatabaseFileUpdateInfo::versionTo() const { return m_sVersionTo; }

const std::string &FhqServerDatabaseFileUpdateInfo::description() const { return m_sDescription; }

// ---------------------------------------------------------------------
// FhqServerDatabaseFileUpdate

FhqServerDatabaseFileUpdate::FhqServerDatabaseFileUpdate(
  const std::string &sVersionFrom, const std::string &sVersionTo, const std::string &sDescription
)
  : m_updateInfo(sVersionFrom, sVersionTo, sDescription) {}

const FhqServerDatabaseFileUpdateInfo &FhqServerDatabaseFileUpdate::info() { return m_updateInfo; };

void FhqServerDatabaseFileUpdate::setWeight(int nWeight) { m_nWeight = nWeight; }

int FhqServerDatabaseFileUpdate::getWeight() { return m_nWeight; }

// ---------------------------------------------------------------------
// FhqServerDatabaseSelectRows

FhqServerDatabaseSelectRows::FhqServerDatabaseSelectRows() { m_pQuery = nullptr; }

FhqServerDatabaseSelectRows::~FhqServerDatabaseSelectRows() {
  if (m_pQuery != nullptr) {
    sqlite3_finalize((sqlite3_stmt *)m_pQuery);
  }
}

void FhqServerDatabaseSelectRows::setQuery(void *pQuery) { m_pQuery = pQuery; }

bool FhqServerDatabaseSelectRows::next() { return sqlite3_step((sqlite3_stmt *)m_pQuery) == SQLITE_ROW; }

std::string FhqServerDatabaseSelectRows::getString(int nColumnNumber) {
  return std::string((const char *)sqlite3_column_text((sqlite3_stmt *)m_pQuery, nColumnNumber));
}

long FhqServerDatabaseSelectRows::getLong(int nColumnNumber) {
  return sqlite3_column_int64((sqlite3_stmt *)m_pQuery, nColumnNumber);
}

// ---------------------------------------------------------------------
// FhqServerDatabaseSqlQuery

FhqServerDatabaseSqlQuery::FhqServerDatabaseSqlQuery(
  FhqServerDatabaseSqlQueryType nSqlType, const std::string &sSqlTable
) {
  m_nSqlType = nSqlType;
  m_sSqlTable = sSqlTable;
  m_bValid = true;
  if (m_nSqlType == FhqServerDatabaseSqlQueryType::SELECT) {
    m_sSqlQuery0 = "SELECT ";
    m_sSqlQuery1 = " FROM " + sSqlTable;
    m_sSqlQuery2 = "";
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::INSERT) {
    m_sSqlQuery0 = "INSERT INTO " + sSqlTable + "(";
    m_sSqlQuery1 = ") VALUES (";
    m_sSqlQuery2 = ");";
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::UPDATE) {
    m_sSqlQuery0 = "UPDATE " + sSqlTable + " SET ";
    m_sSqlQuery1 = " WHERE ";
  } else {
    m_sErrorMessage = "Unknown sql type";
    m_bValid = false;
  }
}

bool FhqServerDatabaseSqlQuery::sel(const std::string &sColumnName) {
  if (!checkName(sColumnName)) {
    return false;
  }
  m_sSqlQuery0 += sColumnName + ", ";
}

bool FhqServerDatabaseSqlQuery::add(const std::string &sColumnName, const std::string &sValue) {
  if (!checkName(sColumnName)) {
    return false;
  }
  if (m_nSqlType == FhqServerDatabaseSqlQueryType::SELECT) {
    m_sErrorMessage = "For select you could not use 'add'";
    m_bValid = false;
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::INSERT) {
    m_sSqlQuery0 += sColumnName + ", ";
    m_sSqlQuery1 += prepareStringValue(sValue) + ", ";
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::UPDATE) {
    m_sSqlQuery0 += sColumnName + " = " + prepareStringValue(sValue);
  } else {
    m_sErrorMessage = "Unknown sql type";
    m_bValid = false;
  }

  // m_sSqlQuery = sBefore + sEscapedValue + sAfter;

  //   while (true) {
  //   /* Locate the substring to replace. */
  //   index = str.find(sName, index);
  //   if (index == std::string::npos)
  //     return false;

  //   /* Make the replacement. */
  //   str.replace(index, 3, "def");

  //   /* Advance index forward so the next iteration doesn't pick it up as well. */
  //   index += 3;
  // }
  return true;
}

bool FhqServerDatabaseSqlQuery::add(const std::string &sColumnName, int nValue) {
  if (!checkName(sColumnName)) {
    return false;
  }

  if (m_nSqlType == FhqServerDatabaseSqlQueryType::SELECT) {
    m_sErrorMessage = "For select you could not use 'add'";
    m_bValid = false;
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::INSERT) {
    m_sSqlQuery0 += sColumnName + ", ";
    m_sSqlQuery1 += std::to_string(nValue) + ", ";
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::UPDATE) {
    m_sSqlQuery0 += sColumnName + " = " + std::to_string(nValue);
  }

  return true;
}

bool FhqServerDatabaseSqlQuery::add(const std::string &sColumnName, long nValue) {
  if (!checkName(sColumnName)) {
    return false;
  }

  if (m_nSqlType == FhqServerDatabaseSqlQueryType::SELECT) {
    m_sErrorMessage = "For select you could not use 'add'";
    m_bValid = false;
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::INSERT) {
    m_sSqlQuery0 += sColumnName + ", ";
    m_sSqlQuery1 += std::to_string(nValue) + ", ";
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::UPDATE) {
    m_sSqlQuery0 += sColumnName + " = " + std::to_string(nValue);
  }

  return true;
}

bool FhqServerDatabaseSqlQuery::where(const std::string &sColumnName, const std::string &sValue) {
  if (!checkName(sColumnName)) {
    return false;
  }
  if (m_nSqlType == FhqServerDatabaseSqlQueryType::SELECT) {
    m_sSqlQuery2 += sColumnName + " = " + prepareStringValue(sValue);
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::INSERT) {
    m_sErrorMessage = "where can be in insert";
    return false;
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::UPDATE) {
    m_sSqlQuery1 += sColumnName + " = " + prepareStringValue(sValue);
  }

  return true;
}

bool FhqServerDatabaseSqlQuery::where(const std::string &sColumnName, int nValue) {
  if (!checkName(sColumnName)) {
    return false;
  }
  if (m_nSqlType == FhqServerDatabaseSqlQueryType::SELECT) {
    m_sSqlQuery2 += sColumnName + " = " + std::to_string(nValue);
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::INSERT) {
    m_sErrorMessage = "where can be in insert";
    return false;
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::UPDATE) {
    m_sSqlQuery1 += sColumnName + " = " + std::to_string(nValue);
  }
}

bool FhqServerDatabaseSqlQuery::where(const std::string &sColumnName, long nValue) {
  if (!checkName(sColumnName)) {
    return false;
  }
  if (m_nSqlType == FhqServerDatabaseSqlQueryType::SELECT) {
    m_sSqlQuery2 += sColumnName + " = " + std::to_string(nValue);
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::INSERT) {
    m_sErrorMessage = "where can be in insert";
    return false;
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::UPDATE) {
    m_sSqlQuery1 += sColumnName + " = " + std::to_string(nValue);
  }
  return true;
}

std::string FhqServerDatabaseSqlQuery::getSql() {
  std::string sSqlQuery = "";
  size_t size0 = m_sSqlQuery0.size();
  size_t size1 = m_sSqlQuery1.size();
  size_t size2 = m_sSqlQuery2.size();
  if (m_nSqlType == FhqServerDatabaseSqlQueryType::SELECT) {
    // TODO refactor this to vector and join
    sSqlQuery = m_sSqlQuery0;
    if (size0 > 2 && m_sSqlQuery0[size0 - 1] == ' ' && m_sSqlQuery0[size0 - 2] == ',') {
      sSqlQuery += m_sSqlQuery0.substr(0, size0 - 2);
    }
    sSqlQuery += m_sSqlQuery1;
    if (size2 > 2 && m_sSqlQuery2[size2 - 1] == ' ' && m_sSqlQuery2[size2 - 2] == ',') {
      sSqlQuery += m_sSqlQuery2.substr(0, size2 - 2);
    }
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::INSERT) {
    if (size0 > 2 && m_sSqlQuery0[size0 - 1] == ' ' && m_sSqlQuery0[size0 - 2] == ',') {
      sSqlQuery += m_sSqlQuery0.substr(0, size0 - 2);
    }
    if (size1 > 2 && m_sSqlQuery1[size1 - 1] == ' ' && m_sSqlQuery1[size1 - 2] == ',') {
      sSqlQuery += m_sSqlQuery1.substr(0, size1 - 2);
    }
    sSqlQuery += m_sSqlQuery2;
  } else if (m_nSqlType == FhqServerDatabaseSqlQueryType::UPDATE) {
    sSqlQuery = m_sSqlQuery0 + m_sSqlQuery1 + m_sSqlQuery2;
  }
  return sSqlQuery;
}

bool FhqServerDatabaseSqlQuery::isValid() { return m_bValid; }

std::string FhqServerDatabaseSqlQuery::getErrorMessage() { return m_sErrorMessage; }

bool FhqServerDatabaseSqlQuery::checkName(const std::string &sColumnName) {
  if (sColumnName.size() < 2) {
    m_sErrorMessage = "Parameter '" + sColumnName + "' must more than 2 characters";
    m_bValid = false;
    return false;
  }
  // TODO check alphabet

  // if (sName[0] != ':') {
  //   m_sErrorMessage = "Parameter '" + sName + "' must starts with ':'";
  //   m_bValid = false;
  //   return false;
  // }
  // nIndex = m_sSqlQuery.find(sName, 0);
  // if (nIndex == std::string::npos) {
  //   m_sErrorMessage = "Not found '" + sName + "' in " + m_sSqlQuery;
  //   m_bValid = false;
  //   return false;
  // }
  return true;
}

std::string FhqServerDatabaseSqlQuery::prepareStringValue(const std::string &sValue) {
  // escaping simbols NUL (ASCII 0), \n, \r, \, ', ", и Control-Z.
  std::string sResult;
  sResult.reserve(sValue.size() * 2);
  sResult.push_back('\'');
  for (int i = 0; i < sValue.size(); i++) {
    char c = sValue[i];
    if (c == '\n') {
      sResult.push_back('\\');
      sResult.push_back('n');
    } else if (c == '\r') {
      sResult.push_back('\\');
      sResult.push_back('r');
    } else if (c == '\\' || c == '"') {
      sResult.push_back('\\');
      sResult.push_back(c);
    } else if (c == '\'') {
      sResult.push_back('\'');
      sResult.push_back(c);
    } else if (c == 0) {
      sResult.push_back('\\');
      sResult.push_back('0');
    } else {
      sResult.push_back(c);
    }
  }
  sResult.push_back('\'');
  return sResult;
}

// ---------------------------------------------------------------------
// FhqServerDatabaseSqlQuerySelect

FhqServerDatabaseSqlQuerySelect::FhqServerDatabaseSqlQuerySelect(const std::string &sSqlTable)
  : FhqServerDatabaseSqlQuery(FhqServerDatabaseSqlQueryType::SELECT, sSqlTable) {}

// ---------------------------------------------------------------------
// FhqServerDatabaseSqlQueryInsert

FhqServerDatabaseSqlQueryInsert::FhqServerDatabaseSqlQueryInsert(const std::string &sSqlTable)
  : FhqServerDatabaseSqlQuery(FhqServerDatabaseSqlQueryType::INSERT, sSqlTable) {}

// ---------------------------------------------------------------------
// FhqServerDatabaseSqlQueryUpdate

FhqServerDatabaseSqlQueryUpdate::FhqServerDatabaseSqlQueryUpdate(const std::string &sSqlTable)
  : FhqServerDatabaseSqlQuery(FhqServerDatabaseSqlQueryType::UPDATE, sSqlTable) {}

// ---------------------------------------------------------------------
// FhqServerDatabaseFile

FhqServerDatabaseFile::FhqServerDatabaseFile(const std::string &sFilename) {
  TAG = "FhqServerDatabaseFile-" + sFilename;
  m_pDatabaseFile = nullptr;
  m_sFilename = sFilename;
  m_nLastBackupTime = 0;
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  std::string sFileStoragePath = pGlobalSettings->get("file_storage").getDirPathValue();
  std::string sDatabaseDir = sFileStoragePath + "/db";
  if (!WsjcppCore::dirExists(sDatabaseDir)) {
    !WsjcppCore::makeDir(sDatabaseDir);
  }
  m_sFileFullpath = sDatabaseDir + "/" + m_sFilename;

  std::string sDatabaseBackupDir = sDatabaseDir + "/backups";
  if (!WsjcppCore::dirExists(sDatabaseBackupDir)) {
    !WsjcppCore::makeDir(sDatabaseBackupDir);
  }
  m_sBaseFileBackupFullpath = sDatabaseBackupDir + "/" + m_sFilename;
};

FhqServerDatabaseFile::~FhqServerDatabaseFile() {
  if (m_pDatabaseFile != nullptr) {
    sqlite3 *db = (sqlite3 *)m_pDatabaseFile;
    sqlite3_close(db);
  }
}

std::string FhqServerDatabaseFile::getFilename() { return m_sFilename; }

std::string FhqServerDatabaseFile::getFileFullpath() { return m_sFileFullpath; }

bool FhqServerDatabaseFile::open() {
  // TODO if ccould not open but has backup try open backup
  // open connection to a DB
  sqlite3 *db = (sqlite3 *)m_pDatabaseFile;
  int nRet = sqlite3_open_v2(m_sFileFullpath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
  if (nRet != SQLITE_OK) {
    WsjcppLog::throw_err(TAG, "Failed to open conn: " + std::to_string(nRet));
    return false;
  }
  m_pDatabaseFile = db;

  const std::string sSqlCheckVersionTable =
    "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='db_version';";

  int nCount = selectSumOrCount(sSqlCheckVersionTable.c_str());
  if (nCount == 0) {
    // create db_version
    const std::string sSqlCreateDbVersion = "CREATE TABLE IF NOT EXISTS db_version ( "
                                            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                            "  version_from VARCHAR(64),"
                                            "  version_to VARCHAR(64),"
                                            "  dt INTEGER NOT NULL,"
                                            "  description VARCHAR(2048) NOT NULL"
                                            ");";
    char *zErrMsg = 0;
    nRet = sqlite3_exec(db, sSqlCreateDbVersion.c_str(), 0, 0, &zErrMsg);
    if (nRet != SQLITE_OK) {
      WsjcppLog::throw_err(TAG, "Problem with create table: " + std::string(zErrMsg));
      return false;
    }
    WsjcppLog::info(TAG, "Created table db_version in " + m_sFileFullpath);
  }

  if (!installUpdates()) {
    WsjcppLog::throw_err(TAG, "Problem with install updates");
    return false;
  }

  WsjcppLog::ok(TAG, "Opened database file " + m_sFileFullpath);
  copyDatabaseToBackup();
  return true;
}

bool FhqServerDatabaseFile::executeQuery(std::string sSqlInsert) {
  copyDatabaseToBackup();
  char *zErrMsg = 0;
  sqlite3 *db = (sqlite3 *)m_pDatabaseFile;
  int nRet = sqlite3_exec(db, sSqlInsert.c_str(), 0, 0, &zErrMsg);
  if (nRet != SQLITE_OK) {
    WsjcppLog::throw_err(TAG, "Problem with insert: " + std::string(zErrMsg) + "\n SQL-query: " + sSqlInsert);
    return false;
  }
  return true;
}

int FhqServerDatabaseFile::selectSumOrCount(std::string sSqlSelectCount) {
  // copyDatabaseToBackup();
  sqlite3 *db = (sqlite3 *)m_pDatabaseFile;
  sqlite3_stmt *pQuery = nullptr;
  int ret = sqlite3_prepare_v2(db, sSqlSelectCount.c_str(), -1, &pQuery, NULL);
  // prepare the statement
  if (ret != SQLITE_OK) {
    WsjcppLog::throw_err(
      TAG, "Failed to prepare select count: " + std::string(sqlite3_errmsg(db)) + "\n SQL-query: " + sSqlSelectCount
    );
  }
  // step to 1st row of data
  ret = sqlite3_step(pQuery);
  if (ret != SQLITE_ROW) { // see documentation, this can return more values as success
    WsjcppLog::throw_err(
      TAG,
      "Failed to step for select count or sum: " + std::string(sqlite3_errmsg(db)) + "\n SQL-query: " + sSqlSelectCount
    );
  }
  int nRet = sqlite3_column_int(pQuery, 0);
  if (pQuery != nullptr)
    sqlite3_finalize(pQuery);
  return nRet;
}

bool FhqServerDatabaseFile::selectRows(std::string sSqlSelectRows, FhqServerDatabaseSelectRows &selectRows) {
  // copyDatabaseToBackup();
  sqlite3 *db = (sqlite3 *)m_pDatabaseFile;
  sqlite3_stmt *pQuery = nullptr;
  int nRet = sqlite3_prepare_v2(db, sSqlSelectRows.c_str(), -1, &pQuery, NULL);
  // prepare the statement
  if (nRet != SQLITE_OK) {
    WsjcppLog::throw_err(
      TAG, "Failed to prepare select rows: " + std::string(sqlite3_errmsg(db)) + "\n SQL-query: " + sSqlSelectRows
    );
    return false;
  }
  selectRows.setQuery((void *)pQuery);
  return true;
}

bool FhqServerDatabaseFile::installUpdates() {
  sqlite3 *db = (sqlite3 *)m_pDatabaseFile;

  // Installed updates
  std::vector<FhqServerDatabaseFileUpdateInfo> installedUpdates;
  {
    sqlite3_stmt *pQuery = nullptr;
    const std::string sSqlCurrentVersion =
      "SELECT version_from, version_to, description FROM db_version ORDER BY rowid";
    int ret = sqlite3_prepare_v2(db, sSqlCurrentVersion.c_str(), -1, &pQuery, NULL);
    if (ret != SQLITE_OK) {
      WsjcppLog::throw_err(
        TAG, "Failed to prepare: " + std::string(sqlite3_errmsg(db)) + "\n SQL-query: " + sSqlCurrentVersion
      );
      return false;
    }
    ret = sqlite3_step(pQuery);
    while (ret == SQLITE_ROW) {
      FhqServerDatabaseFileUpdateInfo info(
        std::string((const char *)sqlite3_column_text(pQuery, 0)), // from
        std::string((const char *)sqlite3_column_text(pQuery, 1)), // to
        std::string((const char *)sqlite3_column_text(pQuery, 2))  // secr
      );
      installedUpdates.push_back(info);
      ret = sqlite3_step(pQuery);
    }
    if (pQuery != nullptr)
      sqlite3_finalize(pQuery);
  }

  std::vector<std::string> installedVersionsTo;
  installedVersionsTo.push_back(""); // start version
  for (int i = 0; i < installedUpdates.size(); i++) {
    installedVersionsTo.push_back(installedUpdates[i].versionTo());
  }

  // install updates
  bool bInstalledNewUpdates = true;
  while (bInstalledNewUpdates) {
    bInstalledNewUpdates = false;
    std::vector<std::string> installedNewUpdates;

    for (int i = 0; i < m_vDbUpdates.size(); i++) {
      FhqServerDatabaseFileUpdate *pUpdate = m_vDbUpdates[i];
      const std::string &sVersionFrom = m_vDbUpdates[i]->info().versionFrom();
      const std::string &sVersionTo = m_vDbUpdates[i]->info().versionTo();

      for (int iv = 0; iv < installedVersionsTo.size(); iv++) {
        if (sVersionFrom == installedVersionsTo[iv]) {
          if (std::find(installedVersionsTo.begin(), installedVersionsTo.end(), sVersionTo) == installedVersionsTo.end()) {
            if (!pUpdate->applyUpdate(this)) {
              return false;
            }
            if (!insertDbVersion(pUpdate->info())) {
              return false;
            }
            WsjcppLog::ok(TAG, "Installed update " + sVersionTo);
            installedNewUpdates.push_back(sVersionTo);
          } else {
            // skip update
          }
        }
      }
    }
    bInstalledNewUpdates = installedNewUpdates.size() > 0;
    for (int i = 0; i < installedNewUpdates.size(); i++) {
      installedVersionsTo.push_back(installedNewUpdates[i]);
    }
  }

  return true;
}

bool FhqServerDatabaseFile::insertDbVersion(const FhqServerDatabaseFileUpdateInfo &info) {
  // TODO escaping
  std::string sSqlDbVersion = "INSERT INTO db_version(version_from, version_to, description, dt) VALUES(\"" +
                              info.versionFrom() + "\", \"" + info.versionTo() + "\", \"" + info.description() +
                              "\", " + std::to_string(WsjcppCore::getCurrentTimeInMilliseconds()) + ")";
  return this->executeQuery(sSqlDbVersion);
}

void FhqServerDatabaseFile::copyDatabaseToBackup() {
  std::lock_guard<std::mutex> lock(m_mutex);
  // every 1 minutes make backup
  int nCurrentTime = WsjcppCore::getCurrentTimeInSeconds();
  if (nCurrentTime - m_nLastBackupTime < 60) {
    return;
  }
  m_nLastBackupTime = nCurrentTime;

  int nMaxBackupsFiles = 9;
  WsjcppLog::info(TAG, "Start backup for " + m_sFileFullpath);
  std::string sFilebackup = m_sBaseFileBackupFullpath + "." + std::to_string(nMaxBackupsFiles);
  if (WsjcppCore::fileExists(sFilebackup)) {
    WsjcppCore::removeFile(sFilebackup);
  }
  for (int i = nMaxBackupsFiles - 1; i >= 0; i--) {
    std::string sFilebackupFrom = m_sBaseFileBackupFullpath + "." + std::to_string(i);
    std::string sFilebackupTo = m_sBaseFileBackupFullpath + "." + std::to_string(i + 1);
    if (WsjcppCore::fileExists(sFilebackupFrom)) {
      if (std::rename(sFilebackupFrom.c_str(), sFilebackupTo.c_str())) {
        WsjcppLog::throw_err(TAG, "Could not rename from " + sFilebackupFrom + " to " + sFilebackupTo);
      }
    }
  }
  sFilebackup = m_sBaseFileBackupFullpath + "." + std::to_string(0);
  if (!WsjcppCore::copyFile(m_sFileFullpath, sFilebackup)) {
    WsjcppLog::throw_err(TAG, "Failed copy file to backup for " + m_sFileFullpath);
  }
  WsjcppLog::info(TAG, "Backup done for " + m_sFileFullpath);
}
