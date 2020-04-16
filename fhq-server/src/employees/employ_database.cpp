#include <employ_database.h>
#include <employees.h>
#include <QThread>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

REGISTRY_WJSCPP_EMPLOY(EmployDatabase)

// ---------------------------------------------------------------------

EmployDatabase::EmployDatabase()
    : WsjcppEmployBase(EmployDatabase::name(), { EmployGlobalSettings::name() }) {
    TAG = EmployDatabase::name();
    
    std::string sGroupDatabase = "database";
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    pGlobalSettings->registrySetting(sGroupDatabase, "storage_type").string("mysql").inFile();
    // TODO validator: 
    // if (!Storages::support(m_sStorageType)) {
    //    WsjcppLog::err(TAG, "Not support storage " + m_sStorageType);
    //    return false;
    //}
    m_nConnectionOutdatedAfterSeconds = 60*60;

    // TODO require some storage_type settings
    pGlobalSettings->registrySetting(sGroupDatabase, "dbhost").string("localhost").inFile();
    pGlobalSettings->registrySetting(sGroupDatabase, "dbport").number(3306).inFile();
    pGlobalSettings->registrySetting(sGroupDatabase, "dbname").string("freehackquest").inFile();
    pGlobalSettings->registrySetting(sGroupDatabase, "dbuser").string("freehackquest_u").inFile();
    pGlobalSettings->registrySetting(sGroupDatabase, "dbpass").password("freehackquest_p").inFile();
    pGlobalSettings->registrySetting(sGroupDatabase, "connection_outdated_after_seconds")
        .number(m_nConnectionOutdatedAfterSeconds).inRuntime().readonly();

    // TODO require some storage_type settings
    // local nosql
    // m_sDatabase_path = "/var/lib/fhq-server/data";
}

// ---------------------------------------------------------------------

bool EmployDatabase::init() {
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

    /*
        WsjcppLog::info(TAG, "Database host: " + m_sDatabase_host);
        WsjcppLog::info(TAG, "Database port: " + std::to_string(m_nDatabase_port));
        WsjcppLog::info(TAG, "Database name: " + m_sDatabase_name);
        WsjcppLog::info(TAG, "Database user: " + m_sDatabase_user);
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
    //    query.exec();
    // }
    
    pGlobalSettings->initFromDatabase(this);

    // TODO
    return true;
}

// ---------------------------------------------------------------------

bool EmployDatabase::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------

bool EmployDatabase::manualCreateDatabase(const std::string& sRootPassword, std::string& sError) {
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
            std::string sDatabaseName = record.value("Database").toString().toStdString();
            if (sDatabaseName == sDatabaseName) {
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
            sError += "Database '" + sDatabaseName + "' already exists. If you wish: you can drop database manually and try again\n";
        }
        if (bUserAlreadyExists) {
            sError += "User '" + sDatabaseUser + "' already exists. If you wish: you can drop user manually and try again\n";
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
        std::string sQuery = "GRANT ALL PRIVILEGES ON " + sDatabaseName + ".* TO '" + sDatabaseUser + "'@'localhost' WITH GRANT OPTION;";
        query.prepare(QString(sQuery.c_str()));
        if (!query.exec()) {
            sError = query.lastError().text().toStdString();
            WsjcppLog::err(TAG, sError);
            return false;
        }
    }
    
    return true;
}

// ---------------------------------------------------------------------

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

    ModelDatabaseConnection *pDBConnection = new ModelDatabaseConnection("qt_sql_default_connection_1_" + QString::number(nThreadID));
    ModelDatabaseConnection *pDBConnection_older = new ModelDatabaseConnection("qt_sql_default_connection_2_" + QString::number(nThreadID));
    m_mDatabaseConnections[nThreadID] = pDBConnection;
    m_mDatabaseConnections_older[nThreadID] = pDBConnection_older;
    pDBConnection->connect();
    return pDBConnection->db();
}

// ---------------------------------------------------------------------
// TODO: it's will be worked only first 8 hours
// - need close connection after hour
// - control of count of connections (must be < 100)

WsjcppStorageConnection *EmployDatabase::getStorageConnection() {
    std::lock_guard<std::mutex> lock(m_mtxStorageConnections);
    
    if (m_vDoRemoveStorageConnections.size() > 0) {
        WsjcppLog::warn(TAG, "TODO cleanup m_vDoRemoveStorageConnections, size = " + std::to_string(m_vDoRemoveStorageConnections.size()));
    }

    std::string sThreadId = WsjcppCore::threadId();
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
        // if connection outdated just reconnect this also maybe need keep several time last connection
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

// ---------------------------------------------------------------------

std::map<std::string, std::string> EmployDatabase::loadAllSettings() {
    std::map<std::string, std::string> vRet;

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM settings");
        query.exec();
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

// ---------------------------------------------------------------------

void EmployDatabase::updateSettingItem(const WsjcppSettingItem *pSettingItem) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("UPDATE settings SET `value` = :value, `group` = :group, `type` = :type WHERE `name` = :name");
    std::string sValue = pSettingItem->convertValueToString(false);
    query.bindValue(":value", QString::fromStdString(sValue));
    query.bindValue(":group", QString::fromStdString(pSettingItem->getGroupName()));
    query.bindValue(":type", QString::fromStdString(pSettingItem->convertTypeToString()));
    query.bindValue(":name", QString::fromStdString(pSettingItem->getName()));

    if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
    }
}

// ---------------------------------------------------------------------

void EmployDatabase::initSettingItem(WsjcppSettingItem *pSettingItem) {
    // StorageConnection *pConn = this->getStorageConnection();

    WsjcppLog::info(TAG, "Init settings to database: " + pSettingItem->getName());
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO settings (`name`, `value`, `group`, `type`) VALUES (:name, :value, :group, :type)");
    query.bindValue(":name", QString::fromStdString(pSettingItem->getName()));
    query.bindValue(":value", QString::fromStdString(pSettingItem->convertValueToString(false)));
    query.bindValue(":group", QString::fromStdString(pSettingItem->getGroupName()));
    query.bindValue(":type", QString::fromStdString(pSettingItem->convertTypeToString()));
    if (!query.exec()) {
        WsjcppLog::throw_err(TAG, query.lastError().text().toStdString());
    }


}

// ---------------------------------------------------------------------