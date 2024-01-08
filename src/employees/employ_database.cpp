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

#include <QSqlQuery>
#include <QSqlRecord>
#include <QThread>
#include <QVariant>
#include <employ_database.h>
#include <employees.h>

// ---------------------------------------------------------------------
// FhqServerDatabaseSelectRows

FhqServerDatabaseSelectRows::FhqServerDatabaseSelectRows() {
    m_pQuery = nullptr;
}

FhqServerDatabaseSelectRows::~FhqServerDatabaseSelectRows() {
    if (m_pQuery != nullptr) {
        sqlite3_finalize(m_pQuery);
    }
}

void FhqServerDatabaseSelectRows::setQuery(sqlite3_stmt* pQuery) {
    m_pQuery = pQuery;
}

bool FhqServerDatabaseSelectRows::next() {
    return  sqlite3_step(m_pQuery) == SQLITE_ROW;
}

std::string FhqServerDatabaseSelectRows::getString(int nColumnNumber) {
    return std::string((const char *)sqlite3_column_text(m_pQuery, nColumnNumber));
}

long FhqServerDatabaseSelectRows::getLong(int nColumnNumber) {
    return sqlite3_column_int64(m_pQuery, nColumnNumber);
}

// ---------------------------------------------------------------------
// FhqServerDatabaseFile

FhqServerDatabaseFile::FhqServerDatabaseFile(const std::string &sFilename, const std::string &sSqlCreateTable) {
    TAG = "FhqServerDatabaseFile-" + sFilename;
    m_pDatabaseFile = nullptr;
    m_sFilename = sFilename;
    m_nLastBackupTime = 0;
    m_sSqlCreateTable = sSqlCreateTable;
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
        sqlite3_close(m_pDatabaseFile);
    }
}

bool FhqServerDatabaseFile::open() {
    // open connection to a DB
    int nRet = sqlite3_open_v2(
        m_sFileFullpath.c_str(),
        &m_pDatabaseFile,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        NULL
    );
    if (nRet != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Failed to open conn: " + std::to_string(nRet));
        return false;
    }
    // Run the SQL (convert the string to a C-String with c_str() )
    char *zErrMsg = 0;
    nRet = sqlite3_exec(m_pDatabaseFile, m_sSqlCreateTable.c_str(), 0, 0, &zErrMsg);
    if (nRet != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Problem with create table: " + std::string(zErrMsg));
        return false;
    }
    WsjcppLog::ok(TAG, "Opened database file " + m_sFileFullpath);
    copyDatabaseToBackup();
    return true;
}

bool FhqServerDatabaseFile::executeQuery(std::string sSqlInsert) {
    copyDatabaseToBackup();
    char *zErrMsg = 0;
    int nRet = sqlite3_exec(m_pDatabaseFile, sSqlInsert.c_str(), 0, 0, &zErrMsg);
    if (nRet != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Problem with insert: " + std::string(zErrMsg) + "\n SQL-query: " + sSqlInsert);
        return false;
    }
    return true;
}

int FhqServerDatabaseFile::selectSumOrCount(std::string sSqlSelectCount) {
    copyDatabaseToBackup();
    sqlite3_stmt* pQuery = nullptr;
    int ret = sqlite3_prepare_v2(m_pDatabaseFile, sSqlSelectCount.c_str(), -1, &pQuery, NULL);
    // prepare the statement
    if (ret != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Failed to prepare select count: " + std::string(sqlite3_errmsg(m_pDatabaseFile)) + "\n SQL-query: " + sSqlSelectCount);
    }
    // step to 1st row of data
    ret = sqlite3_step(pQuery);
    if (ret != SQLITE_ROW) { // see documentation, this can return more values as success
        WsjcppLog::throw_err(TAG, "Failed to step for select count or sum: " + std::string(sqlite3_errmsg(m_pDatabaseFile)) + "\n SQL-query: " + sSqlSelectCount);
    }
    int nRet = sqlite3_column_int(pQuery, 0);
    if (pQuery != nullptr) sqlite3_finalize(pQuery);
    return nRet;
}

bool FhqServerDatabaseFile::selectRows(std::string sSqlSelectRows, FhqServerDatabaseSelectRows &selectRows) {
    copyDatabaseToBackup();
    sqlite3_stmt* pQuery = nullptr;
    int nRet = sqlite3_prepare_v2(m_pDatabaseFile, sSqlSelectRows.c_str(), -1, &pQuery, NULL);
    // prepare the statement
    if (nRet != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Failed to prepare select rows: " + std::string(sqlite3_errmsg(m_pDatabaseFile)) + "\n SQL-query: " + sSqlSelectRows);
        return false;
    }
    selectRows.setQuery(pQuery);
    return true;
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
        std::string sFilebackupTo = m_sBaseFileBackupFullpath + "." + std::to_string(i+1);
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

// ---------------------------------------------------------------------
// EmployDatabase

REGISTRY_WJSCPP_EMPLOY(EmployDatabase)

EmployDatabase::EmployDatabase() : WsjcppEmployBase(EmployDatabase::name(), {EmployGlobalSettings::name()}) {
  TAG = EmployDatabase::name();

  std::string sGroupDatabase = "database";
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  pGlobalSettings->registrySetting(sGroupDatabase, "storage_type").string("mysql").inFile();
  // TODO validator:
  // if (!Storages::support(m_sStorageType)) {
  //    WsjcppLog::err(TAG, "Not support storage " + m_sStorageType);
  //    return false;
  //}
  m_nConnectionOutdatedAfterSeconds = 60 * 60;

  // TODO require some storage_type settings
  pGlobalSettings->registrySetting(sGroupDatabase, "dbhost").string("localhost").inFile();
  pGlobalSettings->registrySetting(sGroupDatabase, "dbport").number(3306).inFile();
  pGlobalSettings->registrySetting(sGroupDatabase, "dbname").string("freehackquest").inFile();
  pGlobalSettings->registrySetting(sGroupDatabase, "dbuser").string("freehackquest_u").inFile();
  pGlobalSettings->registrySetting(sGroupDatabase, "dbpass").password("freehackquest_p").inFile();
  pGlobalSettings->registrySetting(sGroupDatabase, "connection_outdated_after_seconds")
      .number(m_nConnectionOutdatedAfterSeconds)
      .inRuntime()
      .readonly();

  // TODO require some storage_type settings
  // local nosql
  // m_sDatabase_path = "/var/lib/fhq-server/data";
}

bool EmployDatabase::init() {
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  /*
      WsjcppLog::info(TAG, "Database host: " + m_sDatabase_host);
      WsjcppLog::info(TAG, "Database port: " +
     std::to_string(m_nDatabase_port)); WsjcppLog::info(TAG, "Database name: " +
     m_sDatabase_name); WsjcppLog::info(TAG, "Database user: " +
     m_sDatabase_user);
   */
  m_sStorageType = pGlobalSettings->get("storage_type").getStringValue();
  if (!WsjcppStorages::support(m_sStorageType)) {
    WsjcppLog::err(TAG, "Not support storage " + m_sStorageType);
    return false;
  }
  m_pStorage = WsjcppStorages::create(m_sStorageType);
  // TODO redesign init in global settings
  if (!m_pStorage->applyConfigFromFile(pGlobalSettings->getFilepathConf())) {
    return false;
  }

  /*if (!StorageUpdates::apply(m_pStorage)) {
      return false;
  }*/
  WsjcppStorageUpdates::apply2(m_pStorage);

  // deprecated
  m_pDBConnection = new ModelDatabaseConnection("qt_sql_default_connection_1");
  m_pDBConnection_older = new ModelDatabaseConnection("qt_sql_default_connection_2");

  if (!m_pDBConnection->connect()) {
    return false;
  }

  // TODO: redesign
  // cleanup old user tokens
  // {
  //    QSqlDatabase db = *(m_pDBConnection->db());
  //    QSqlQuery query(db);
  //    query.prepare("DELETE FROM users_tokens WHERE end_date < NOW()");
  //    if (!query.exec()) {
  //
  //
  //    }
  // }

  pGlobalSettings->initFromDatabase(this);

  // new database format
  int nRet = 0;
  if (SQLITE_OK != (nRet = sqlite3_initialize())) {
    WsjcppLog::throw_err(TAG, "Failed to initialize build-in sqlite3 library: " + std::to_string(nRet));
    return false;
  }
  WsjcppLog::ok(TAG, "Initialize build-in sqlite3 library");
  if (!this->initUsefulLinksDatabase()) {
    return false;
  }

  // TODO
  return true;
}

bool EmployDatabase::deinit() {
  // TODO
  return true;
}

bool EmployDatabase::manualCreateDatabase(const std::string &sRootPassword, std::string &sError) {
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  m_sStorageType = pGlobalSettings->get("storage_type").getStringValue();
  if (!WsjcppStorages::support(m_sStorageType)) {
    WsjcppLog::err(TAG, "Not support storage " + m_sStorageType);
    return false;
  }
  m_pStorage = WsjcppStorages::create(m_sStorageType);
  std::string sDatabaseHost = pGlobalSettings->get("dbhost").getStringValue();
  int nDatabasePort = pGlobalSettings->get("dbport").getNumberValue();
  std::string sDatabaseName = pGlobalSettings->get("dbname").getStringValue();
  std::string sDatabaseUser = pGlobalSettings->get("dbuser").getStringValue();
  std::string sDatabasePass = pGlobalSettings->get("dbpass").getPasswordValue();
  // m_pStorage->connect()

  QSqlDatabase *pDatabase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "manualCreateDatabase"));

  pDatabase->setHostName(QString::fromStdString(sDatabaseHost));
  pDatabase->setPort(nDatabasePort);
  pDatabase->setUserName("root");
  pDatabase->setPassword(QString::fromStdString(sRootPassword));
  if (!pDatabase->open()) {
    sError = "Could not connect to mysql://" + sDatabaseHost + ":" + std::to_string(nDatabasePort);
    WsjcppLog::err(TAG, sError);
    WsjcppLog::err(TAG, "Maybe wrong root password");
    return false;
  }
  WsjcppLog::info(TAG, "Success connected");

  // check the database exists
  bool bDatabaseAlreadyExists = false;
  {
    QSqlQuery query(*pDatabase);
    query.prepare("SHOW DATABASES;");
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      WsjcppLog::err(TAG, sError);
      return false;
    }
    while (query.next()) {
      QSqlRecord record = query.record();
      std::string sDatabaseName1 = record.value("Database").toString().toStdString();
      if (sDatabaseName == sDatabaseName1) {
        bDatabaseAlreadyExists = true;
      }
    }
  }

  // check that user exists
  bool bUserAlreadyExists = false;
  {
    QSqlQuery query(*pDatabase);
    // TODO escaping
    std::string sQuery = "select user from mysql.user where user = '" + sDatabaseUser + "';";
    query.prepare(QString(sQuery.c_str()));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      WsjcppLog::err(TAG, sError);
      return false;
    }
    while (query.next()) {
      QSqlRecord record = query.record();
      std::string sUserName = record.value("user").toString().toStdString();
      if (sUserName == sDatabaseUser) {
        bUserAlreadyExists = true;
      }
    }
  }

  if (bDatabaseAlreadyExists || bUserAlreadyExists) {
    sError = "";
    if (bDatabaseAlreadyExists) {
      sError += "Database '" + sDatabaseName +
                "' already exists. If you wish: you can drop database manually "
                "and try again\n";
    }
    if (bUserAlreadyExists) {
      sError += "User '" + sDatabaseUser +
                "' already exists. If you wish: you can drop user manually and "
                "try again\n";
    }
    return false;
  }

  // try create database
  {
    QSqlQuery query(*pDatabase);
    // TODO escaping
    std::string sQuery = "CREATE DATABASE `" + sDatabaseName + "` CHARACTER SET utf8 COLLATE utf8_general_ci;";
    query.prepare(QString(sQuery.c_str()));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      WsjcppLog::err(TAG, sError);
      return false;
    }
  }

  // try create user
  {
    QSqlQuery query(*pDatabase);
    // TODO escaping
    std::string sQuery = "CREATE USER '" + sDatabaseUser + "'@'localhost' IDENTIFIED BY '" + sDatabasePass + "';";
    query.prepare(QString(sQuery.c_str()));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      WsjcppLog::err(TAG, sError);
      return false;
    }
  }

  // apply priveleges
  {
    QSqlQuery query(*pDatabase);
    // TODO escaping
    std::string sQuery =
        "GRANT ALL PRIVILEGES ON " + sDatabaseName + ".* TO '" + sDatabaseUser + "'@'localhost' WITH GRANT OPTION;";
    query.prepare(QString(sQuery.c_str()));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      WsjcppLog::err(TAG, sError);
      return false;
    }
  }

  return true;
}

QSqlDatabase *EmployDatabase::database() {
  // swap connection
  std::lock_guard<std::mutex> lock(m_mtxSwapConenctions);
  long long nThreadID = (long long)QThread::currentThreadId();

  if (m_mDatabaseConnections.contains(nThreadID)) {
    ModelDatabaseConnection *pDBConnection = m_mDatabaseConnections[nThreadID];
    ModelDatabaseConnection *pDBConnection_older = m_mDatabaseConnections_older[nThreadID];

    if (pDBConnection->isOutdated()) {
      pDBConnection_older->close();
      pDBConnection_older->swap(pDBConnection);
      pDBConnection->connect();
    }
    return pDBConnection->db();
  }

  ModelDatabaseConnection *pDBConnection =
      new ModelDatabaseConnection("qt_sql_default_connection_1_" + QString::number(nThreadID));
  ModelDatabaseConnection *pDBConnection_older =
      new ModelDatabaseConnection("qt_sql_default_connection_2_" + QString::number(nThreadID));
  m_mDatabaseConnections[nThreadID] = pDBConnection;
  m_mDatabaseConnections_older[nThreadID] = pDBConnection_older;
  pDBConnection->connect();
  return pDBConnection->db();
}

// TODO: it's will be worked only first 8 hours
// - need close connection after hour
// - control of count of connections (must be < 100)

WsjcppStorageConnection *EmployDatabase::getStorageConnection() {
  std::lock_guard<std::mutex> lock(m_mtxStorageConnections);

  if (m_vDoRemoveStorageConnections.size() > 0) {
    WsjcppLog::warn(TAG, "TODO cleanup m_vDoRemoveStorageConnections, size = " +
                             std::to_string(m_vDoRemoveStorageConnections.size()));
  }

  std::string sThreadId = WsjcppCore::getThreadId();
  WsjcppStorageConnection *pStorageConnection = nullptr;
  std::map<std::string, WsjcppStorageConnection *>::iterator it;
  it = m_mapStorageConnections.find(sThreadId);
  if (it == m_mapStorageConnections.end()) {
    pStorageConnection = m_pStorage->connect();
    if (pStorageConnection == nullptr) {
      return nullptr;
    }
    m_mapStorageConnections[sThreadId] = pStorageConnection;
  } else {
    pStorageConnection = it->second;
    // if connection outdated just reconnect this also maybe need keep several
    // time last connection
    if (pStorageConnection->getConnectionDurationInSeconds() > m_nConnectionOutdatedAfterSeconds) {
      m_vDoRemoveStorageConnections.push_back(pStorageConnection);
      pStorageConnection = m_pStorage->connect();
      if (pStorageConnection == nullptr) {
        return nullptr;
      }
      m_mapStorageConnections[sThreadId] = pStorageConnection;
    }
  }
  return pStorageConnection;
}

std::map<std::string, std::string> EmployDatabase::loadAllSettings() {
  std::map<std::string, std::string> vRet;

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());

  {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM settings");
    if (!query.exec()) {
      WsjcppLog::throw_err(TAG, "EmployDatabase::loadAllSettings => " + query.lastError().text().toStdString());
      return vRet;
    };
    while (query.next()) {
      QSqlRecord record = query.record();
      std::string sName = record.value("name").toString().toStdString();
      std::string sGroup = record.value("group").toString().toStdString();
      std::string sValue = record.value("value").toString().toStdString();
      std::string sType = record.value("type").toString().toStdString();
      if (vRet.count(sName) == 0) {
        vRet.insert(std::pair<std::string, std::string>(sName, sValue));
      } else {
        WsjcppLog::err(TAG, "In databases settings found duplicates '" + sName + "'");
      }
    }
  }
  return vRet;
}

void EmployDatabase::updateSettingItem(const WsjcppSettingItem *pSettingItem) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("UPDATE settings SET `value` = :value, `group` = :group, "
                "`type` = :type WHERE `name` = :name");
  std::string sValue = pSettingItem->convertValueToString(false);
  query.bindValue(":value", QString::fromStdString(sValue));
  query.bindValue(":group", QString::fromStdString(pSettingItem->getGroupName()));
  query.bindValue(":type", QString::fromStdString(pSettingItem->convertTypeToString()));
  query.bindValue(":name", QString::fromStdString(pSettingItem->getName()));

  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
  }
}

void EmployDatabase::initSettingItem(WsjcppSettingItem *pSettingItem) {
  // StorageConnection *pConn = this->getStorageConnection();

  WsjcppLog::info(TAG, "Init settings to database: " + pSettingItem->getName());
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("INSERT INTO settings (`name`, `value`, `group`, `type`) "
                "VALUES (:name, :value, :group, :type)");
  query.bindValue(":name", QString::fromStdString(pSettingItem->getName()));
  query.bindValue(":value", QString::fromStdString(pSettingItem->convertValueToString(false)));
  query.bindValue(":group", QString::fromStdString(pSettingItem->getGroupName()));
  query.bindValue(":type", QString::fromStdString(pSettingItem->convertTypeToString()));
  if (!query.exec()) {
    WsjcppLog::throw_err(TAG, query.lastError().text().toStdString());
  }
}

bool EmployDatabase::initUsefulLinksDatabase() {
  // TODO migration
  m_pUsefulLinks = new FhqServerDatabaseFile("useful_links.db",
    "CREATE TABLE IF NOT EXISTS useful_links ( "
    "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
    // "  uuid VARCHAR(36) NOT NULL,"
    "  url VARCHAR(2048) NOT NULL,"
    "  description VARCHAR(2048) NOT NULL,"
    "  author VARCHAR(127) NOT NULL,"
    "  dt INTEGER NOT NULL,"
    "  user_favorites INTEGER NOT NULL,"
    "  user_clicks INTEGER NOT NULL,"
    "  user_comments INTEGER NOT NULL,"
    "  tags VARCHAR(2048) NOT NULL"
    ");"
  );
  if (!m_pUsefulLinks->open()) {
    return false;
  }
  WsjcppLog::ok(TAG, "Initialized useful_links.db");
  return true;
}