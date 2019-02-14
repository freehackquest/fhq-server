#include <mysql_storage.h>
#include <utils_logger.h>
#include <mysql/mysql.h>
#include <parse_config.h>
// #include <fs.h>
// #include <ts.h>

REGISTRY_STORAGE(MySqlStorage)

MySqlStorageConnection::MySqlStorageConnection(MYSQL *pConn) : StorageConnection() {
    m_pConnection = pConn;
    TAG = "MySqlStorageConenction";
}

// ----------------------------------------------------------------------

MySqlStorageConnection::~MySqlStorageConnection() {
    // TODO disconnect
}

// ----------------------------------------------------------------------

std::string MySqlStorageConnection::escapeString(const std::string &sValue) {
    char *to;
    unsigned long len = mysql_real_escape_string_quote(m_pConnection, to, sValue.c_str(),sValue.length(),'"');
    std::string sResult = "\"" + std::string(to,len) + "\"";
    return sResult;
}

// ----------------------------------------------------------------------

bool MySqlStorageConnection::executeQuery(const std::string &sQuery) {
    if (mysql_query(m_pConnection, sQuery.c_str())) {
        std::string sError2(mysql_error(m_pConnection));
        Log::err(TAG, "Problem on executeQuery " + sError2 + "\r\nQuery: " + sQuery);
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------

std::string MySqlStorageConnection::lastDatabaseVersion() {
    /*
    std::string sQuery = "SELECT id, version FROM db_updates ORDER BY id DESC LIMIT 0,1";
    int nCurrVersion = 0;
    if (mysql_query(pConn, sQuery.c_str())) {
        std::string sError(mysql_error(pConn));
        if (sError.find("db_updates' doesn't exist") != std::string::npos) {
            Log::info(TAG, "Creating table db_updates .... ");
            std::string sTableDbUpdates = 
                "CREATE TABLE IF NOT EXISTS db_updates ("
			    "  id int(11) NOT NULL AUTO_INCREMENT,"
                "  version INT DEFAULT NULL,"
                "  dt datetime DEFAULT NULL,"
                "  PRIMARY KEY (id)"
                ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;";
            if (mysql_query(pConn, sTableDbUpdates.c_str())) {
                std::string sError2(mysql_error(pConn));
                Log::err(TAG, "Problem on create table db_updates " + sError2);
                return false;
            } else {
                Log::ok(TAG, "Table db_updates success created");
                nCurrVersion = 1;
            }
        } else {
            Log::err(TAG, "Problem with database " + sError);
            return false;
        }
    } else {
        MYSQL_RES *pRes = mysql_use_result(pConn);
        MYSQL_ROW row;
        if ((row = mysql_fetch_row(pRes)) != NULL) {
            nCurrVersion = std::stol(std::string(row[1]));
        } else {
            nCurrVersion = 1;
        }
        mysql_free_result(pRes);
    }
    */

    std::string sLastVersion = "";
    std::string sQuery = "SELECT version FROM updates ORDER BY id DESC LIMIT 0,1";

    if (mysql_query(m_pConnection, sQuery.c_str())) {
        Log::err(TAG, "Error SELECT version FROM updates: " + std::string(mysql_error(m_pConnection)));
    } else {
        MYSQL_RES *pRes = mysql_use_result(m_pConnection);
        MYSQL_ROW row;
        // output table name
        if ((row = mysql_fetch_row(pRes)) != NULL) {
            sLastVersion = std::string(row[0]);
        }
        mysql_free_result(pRes);
    }
    delete m_pConnection;
    return sLastVersion;
}

// ----------------------------------------------------------------------

bool MySqlStorageConnection::insertUpdateInfo(const std::string &sVersion, const std::string &sDescription) {
    std::string sInsertNewVersion = "INSERT INTO updates(version, description, datetime_update) "
        " VALUES(" + escapeString(sVersion) + ", " + escapeString(sDescription) + ",NOW());";
    if (mysql_query(m_pConnection, sInsertNewVersion.c_str())) {
        Log::err(TAG, "Could not insert row to updates: " + std::string(mysql_error(m_pConnection)));
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------

MySqlStorage::MySqlStorage() {
    TAG = "MySqlStorage";
    m_sDatabaseHost = "";
    m_sDatabaseName = "";
    m_sDatabaseUser = "";
    m_sDatabasePass = "";
    m_nDatabasePort = 3306;
}

// ----------------------------------------------------------------------

bool MySqlStorage::applyConfigFromFile(const std::string &sFilePath) {
    ParseConfig parseConfig(sFilePath);
	parseConfig.load();
    if (!parseConfig.has("dbhost")) {
        Log::err(TAG, "Not found 'dbhost' in " + sFilePath);
        return false;
    }

    if (!parseConfig.has("dbport")) {
        Log::err(TAG, "Not found 'dbport' in " + sFilePath);
        return false;
    }

    if (!parseConfig.has("dbname")) {
        Log::err(TAG, "Not found 'dbname' in " + sFilePath);
        return false;
    }

    if (!parseConfig.has("dbuser")) {
        Log::err(TAG, "Not found 'dbuser' in " + sFilePath);
        return false;
    }

    if (!parseConfig.has("dbpass")) {
        Log::err(TAG, "Not found 'dbpass' in " + sFilePath);
        return false;
    }

    m_sDatabaseHost = parseConfig.stringValue("dbhost", m_sDatabaseHost);
    m_nDatabasePort = parseConfig.intValue("dbport", m_nDatabasePort);
    m_sDatabaseName = parseConfig.stringValue("dbname", m_sDatabaseName);
    m_sDatabaseUser = parseConfig.stringValue("dbuser", m_sDatabaseUser);
    m_sDatabasePass = parseConfig.stringValue("dbpass", m_sDatabasePass);

    Log::info(TAG, "Database host: " + m_sDatabaseHost);
    Log::info(TAG, "Database port: " + std::to_string(m_nDatabasePort));
    Log::info(TAG, "Database name: " + m_sDatabaseName);
    Log::info(TAG, "Database user: " + m_sDatabaseUser);
    Log::info(TAG, "Database passord: (hided)");

    return true;
}

// ----------------------------------------------------------------------

StorageConnection * MySqlStorage::connect() {
    MySqlStorageConnection *pConn = nullptr;
    MYSQL *pDatabase = mysql_init(NULL);
    if (!mysql_real_connect(pDatabase, 
            m_sDatabaseHost.c_str(),
            m_sDatabaseUser.c_str(),
            m_sDatabasePass.c_str(),
            m_sDatabaseName.c_str(), 
            m_nDatabasePort, NULL, 0)) {
        Log::err(TAG, std::string(mysql_error(pDatabase)));
        Log::err(TAG, "Failed to connect.");
    } else {
        pConn = new MySqlStorageConnection(pDatabase);
    }
    return pConn;
}

// ----------------------------------------------------------------------

void MySqlStorage::clean() {

}

// ----------------------------------------------------------------------

std::vector<std::string> MySqlStorage::prepareSqlQueries(StorageStruct &storageStruct) {
    std::vector<std::string> vRet;
    if (storageStruct.mode() == StorageStructTableMode::ALTER) {
        // drop columns
        std::vector<std::string> vDropColumns = storageStruct.listDropColumns();
        for (int i = 0; i < vDropColumns.size(); i++) {
            vRet.push_back("ALTER TABLE " + storageStruct.tableName() + " DROP COLUMN " + vDropColumns[i] + ";");
        }
        
        // add columns
        std::vector<StorageStructColumn> vAddColumns = storageStruct.listAddColumns();
        for (int i = 0; i < vAddColumns.size(); i++) {
            vRet.push_back("ALTER TABLE " + storageStruct.tableName() + " ADD COLUMN " + generateLineColumnForSql(vAddColumns[i]) + ";");
        }

        // alter columns
        std::vector<StorageStructColumn> vAlterColumns = storageStruct.listAlterColumns();
        for (int i = 0; i < vAlterColumns.size(); i++) {
            vRet.push_back("ALTER TABLE " + storageStruct.tableName() + " MODIFY " + generateLineColumnForSql(vAlterColumns[i]) + ";");
        }
    } else if (storageStruct.mode() == StorageStructTableMode::DROP) {
        vRet.push_back("DROP TABLE IF EXISTS " + storageStruct.tableName() + ";");
    } else if (storageStruct.mode() == StorageStructTableMode::CREATE) {
        std::string sQuery = "";
        sQuery += "CREATE TABLE IF NOT EXISTS " + storageStruct.tableName() + " (\r\n";
        std::string sPrimaryKeyColumnName = "";
        // add columns
        std::vector<StorageStructColumn> vAddColumns = storageStruct.listAddColumns();
        for (int i = 0; i < vAddColumns.size(); i++) {
            StorageStructColumn c = vAddColumns[i];
            sQuery += "  " + generateLineColumnForSql(c) + ",\r\n";
            if (c.isPrimaryKey()) {
                sPrimaryKeyColumnName = c.columnName();
            }
        }
        sQuery += "  PRIMARY KEY (" + sPrimaryKeyColumnName + ")\r\n";
        sQuery += ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;";
        vRet.push_back(sQuery);
    }
    return vRet;
}

// ----------------------------------------------------------------------

std::string MySqlStorage::generateLineColumnForSql(StorageStructColumn &c) {
    std::string sSqlColumn = "";

    sSqlColumn += c.columnName();
    
    if (c.columnType() == "number") {
        sSqlColumn += " INT";
    } else if (c.columnType() == "string") {
        sSqlColumn += " VARCHAR(" + std::to_string(c.columnTypeSize()) + ")";
    } else if (c.columnType() == "text") {
        sSqlColumn += " TEXT";
    } else if (c.columnType() == "datetime") {
        sSqlColumn += " DATETIME";
    } else {
        Log::err(TAG, "Unknown columnType " + c.columnType());
    }

    if (c.isNotNull()) {
        sSqlColumn += " NOT NULL";
    }

    if (c.isAutoIncrement()) {
        sSqlColumn += " AUTO_INCREMENT";
    }
    return sSqlColumn;
}

// ----------------------------------------------------------------------
