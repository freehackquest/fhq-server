#include <employ_database.h>
#include <wjscpp_employees.h>
#include <QThread>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

REGISTRY_WJSCPP_EMPLOY(EmployDatabase)

// ---------------------------------------------------------------------

EmployDatabase::EmployDatabase()
    : EmployBase(EmployDatabase::name(), {EmployServerConfig::name(), EmployGlobalSettings::name()}) {
    TAG = EmployDatabase::name();
    
    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();
    pGlobalSettings->regestrySetting("storage_type").string("mysql").inFile();
    // TODO validator: 
    // if (!Storages::support(m_sStorageType)) {
    //    Log::err(TAG, "Not support storage " + m_sStorageType);
    //    return false;
    //}

    // TODO require some storage_type settings
    pGlobalSettings->regestrySetting("dbhost").string("localhost").inFile();
    pGlobalSettings->regestrySetting("dbport").number(3306).inFile();
    pGlobalSettings->regestrySetting("dbname").string("freehackquest").inFile();
    pGlobalSettings->regestrySetting("dbuser").string("freehackquest_u").inFile();
    pGlobalSettings->regestrySetting("dbpass").string("freehackquest_p").inFile();
    
    // TODO require some storage_type settings
    // local nosql
    // m_sDatabase_path = "/var/lib/fhq-server/data";
}

// ---------------------------------------------------------------------

bool EmployDatabase::init() {
    EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();

    /*
            Log::info(TAG, "Database host: " + m_sDatabase_host);
        Log::info(TAG, "Database port: " + std::to_string(m_nDatabase_port));
        Log::info(TAG, "Database name: " + m_sDatabase_name);
        Log::info(TAG, "Database user: " + m_sDatabase_user);
     */
    m_sStorageType = pGlobalSettings->get("storage_type").getStringValue();
    if (!Storages::support(m_sStorageType)) {
        Log::err(TAG, "Not support storage " + m_sStorageType);
        return false;
    }
    m_pStorage = Storages::create(m_sStorageType);
    // TODO redesign init in global settings
    if (!m_pStorage->applyConfigFromFile(pServerConfig->filepathConf())) {
        return false;
    }

    /*if (!StorageUpdates::apply(m_pStorage)) {
        return false;
    }*/
    StorageUpdates::apply2(m_pStorage);

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
    
    
    // TODO
    return true;
}

// ---------------------------------------------------------------------

bool EmployDatabase::manualCreateDatabase(const std::string& sRootPassword, std::string& sError) {
    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();

    m_sStorageType = pGlobalSettings->get("storage_type").getStringValue();
    if (!Storages::support(m_sStorageType)) {
        Log::err(TAG, "Not support storage " + m_sStorageType);
        return false;
    }
    m_pStorage = Storages::create(m_sStorageType);
    std::string sDatabaseHost = pGlobalSettings->get("dbhost").getStringValue();
    int nDatabasePort = pGlobalSettings->get("dbport").getNumberValue();
    std::string sDatabaseName = pGlobalSettings->get("dbname").getStringValue();
    std::string sDatabaseUser = pGlobalSettings->get("dbuser").getStringValue();
    std::string sDatabasePass = pGlobalSettings->get("dbpass").getStringValue();
    // m_pStorage->connect()

    QSqlDatabase *pDatabase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "manualCreateDatabase"));
    
    pDatabase->setHostName(QString::fromStdString(sDatabaseHost));
    pDatabase->setPort(nDatabasePort);
    pDatabase->setUserName("root");
    pDatabase->setPassword(QString::fromStdString(sRootPassword));
    if (!pDatabase->open()) {
        sError = "Could not connect to mysql://" + sDatabaseHost + ":" + std::to_string(nDatabasePort);
        Log::err(TAG, sError);
        Log::err(TAG, "Maybe wrong root password");
        return false;
    }
    Log::info(TAG, "Success connected");
    
    // check the database exists
    bool bDatabaseAlreadyExists = false;
    {    
        QSqlQuery query(*pDatabase);
        query.prepare("SHOW DATABASES;");
        if (!query.exec()) {
            sError = query.lastError().text().toStdString();
            Log::err(TAG, sError);
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
            Log::err(TAG, sError);
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
            Log::err(TAG, sError);
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
            Log::err(TAG, sError);
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
            Log::err(TAG, sError);
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

StorageConnection *EmployDatabase::getStorageConnection() {
    std::string sThreadId = Fallen::threadId();
    StorageConnection *pStorageConnection = nullptr;
    std::map<std::string, StorageConnection *>::iterator it;
    it = m_mapStorageConnections.find(sThreadId);
    if (it == m_mapStorageConnections.end()) {
        pStorageConnection = m_pStorage->connect();
        if (pStorageConnection == nullptr) {
            return nullptr;
        }
        m_mapStorageConnections[sThreadId] = pStorageConnection;
    } else {
        pStorageConnection = it->second;
    }
    return pStorageConnection;
}
