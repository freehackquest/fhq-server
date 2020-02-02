#include <storages/mysql/mysql_storage.h>
#include <core/fallen.h>
#include <mysql/mysql.h>

REGISTRY_STORAGE(MySqlStorage)

MySqlStorageConnection::MySqlStorageConnection(MYSQL *pConn, Storage *pStorage) : StorageConnection() {
    m_pConnection = pConn;
    m_pStorage = pStorage;
    TAG = "MySqlStorageConenction";
}

// ----------------------------------------------------------------------

MySqlStorageConnection::~MySqlStorageConnection() {
    mysql_close(m_pConnection);
    // delete m_pConnection;
}

// ----------------------------------------------------------------------

bool MySqlStorageConnection::executeQuery(const std::string &sQuery) {
    // TODO statistics time
    std::lock_guard<std::mutex> lock(m_mtxConn);
    // WSJCppLog::info(TAG, "Try " + sQuery);
    if (mysql_query(m_pConnection, sQuery.c_str())) {
        WSJCppLog::err(TAG, "Problem on executeQuery \r\nQuery: " + sQuery);
        std::string sError(mysql_error(m_pConnection));
        WSJCppLog::err(TAG, "executeQuery error " + sError);
        return false;
    } else {
        // WSJCppLog::ok(TAG, "" + sQuery);
    }
    return true;
}

// ----------------------------------------------------------------------

std::string MySqlStorageConnection::lastDatabaseVersion() {
    std::lock_guard<std::mutex> lock(m_mtxConn);

    std::string sLastVersion = "";
    std::string sQuery = "SELECT version FROM updates ORDER BY id DESC LIMIT 0,1";

    if (mysql_query(m_pConnection, sQuery.c_str())) {
        std::string sError(mysql_error(m_pConnection));
        if (sError.find("updates' doesn't exist") != std::string::npos) {
            WSJCppLog::warn(TAG, "Creating table updates .... ");
            std::string sTableDbUpdates = 
                "CREATE TABLE IF NOT EXISTS updates ("
                "  id INT NOT NULL AUTO_INCREMENT,"
                "  version varchar(255) DEFAULT NULL,"
                "  description text,"
                "  datetime_update datetime DEFAULT NULL,"
                "  PRIMARY KEY (`id`)"
                ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;";
            if (mysql_query(m_pConnection, sTableDbUpdates.c_str())) {
                std::string sError2(mysql_error(m_pConnection));
                WSJCppLog::err(TAG, "Problem on create table updates " + sError2);
                return "error";
            } else {
                WSJCppLog::ok(TAG, "Table updates success created");
                sLastVersion = "";
                return "";
            }
        } else {
            WSJCppLog::err(TAG, "Problem with database " + sError);
            return "error";
        }
    } else {
        MYSQL_RES *pRes = mysql_use_result(m_pConnection);
        MYSQL_ROW row;
        // output table name
        if ((row = mysql_fetch_row(pRes)) != NULL) {
            sLastVersion = std::string(row[0]);
        }
        mysql_free_result(pRes);
    }
    return sLastVersion;
}

// ----------------------------------------------------------------------

std::vector<std::string> MySqlStorageConnection::getInstalledVersions() {
    std::lock_guard<std::mutex> lock(m_mtxConn);
    std::vector<std::string> vVersions;

    std::string sQuery = "SELECT version FROM updates ORDER BY id";

    if (mysql_query(m_pConnection, sQuery.c_str())) {
        std::string sError(mysql_error(m_pConnection));
        if (sError.find("updates' doesn't exist") != std::string::npos) {
            WSJCppLog::warn(TAG, "Creating table updates .... ");
            std::string sTableDbUpdates = 
                "CREATE TABLE IF NOT EXISTS updates ("
                "  id INT NOT NULL AUTO_INCREMENT,"
                "  version varchar(255) DEFAULT NULL,"
                "  description text,"
                "  datetime_update datetime DEFAULT NULL,"
                "  PRIMARY KEY (`id`)"
                ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;";
            if (mysql_query(m_pConnection, sTableDbUpdates.c_str())) {
                std::string sError2(mysql_error(m_pConnection));
                WSJCppLog::throw_err(TAG, "Problem on create table updates " + sError2);
            } else {
                WSJCppLog::ok(TAG, "Table updates success created");
                return vVersions;
            }
        } else {
            WSJCppLog::throw_err(TAG, "Problem with database " + sError);
        }
    } else {
        MYSQL_RES *pRes = mysql_use_result(m_pConnection);
        MYSQL_ROW row;
        // output table name
        while ((row = mysql_fetch_row(pRes)) != NULL) {
            vVersions.push_back(std::string(row[0]));
        }
        mysql_free_result(pRes);
    }
    return vVersions;
}

// ----------------------------------------------------------------------

bool MySqlStorageConnection::insertUpdateInfo(const std::string &sVersion, const std::string &sDescription) {
    std::lock_guard<std::mutex> lock(m_mtxConn);
    std::string sInsertNewVersion = "INSERT INTO updates(version, description, datetime_update) "
        " VALUES(" + m_pStorage->prepareStringValue(sVersion) + ", " + m_pStorage->prepareStringValue(sDescription) + ",NOW());";
    if (mysql_query(m_pConnection, sInsertNewVersion.c_str())) {
        WSJCppLog::err(TAG, "Could not insert row to updates: " + std::string(mysql_error(m_pConnection)));
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
    WJSCppParseConfig parseConfig(sFilePath);
    parseConfig.load();

    if (!parseConfig.has("dbhost")) {
        WSJCppLog::err(TAG, "Not found 'dbhost' in " + sFilePath);
        return false;
    }

    if (!parseConfig.has("dbport")) {
        WSJCppLog::err(TAG, "Not found 'dbport' in " + sFilePath);
        return false;
    }

    if (!parseConfig.has("dbname")) {
        WSJCppLog::err(TAG, "Not found 'dbname' in " + sFilePath);
        return false;
    }

    if (!parseConfig.has("dbuser")) {
        WSJCppLog::err(TAG, "Not found 'dbuser' in " + sFilePath);
        return false;
    }

    if (!parseConfig.has("dbpass")) {
        WSJCppLog::err(TAG, "Not found 'dbpass' in " + sFilePath);
        return false;
    }

    m_sDatabaseHost = parseConfig.stringValue("dbhost", m_sDatabaseHost);
    m_nDatabasePort = parseConfig.intValue("dbport", m_nDatabasePort);
    m_sDatabaseName = parseConfig.stringValue("dbname", m_sDatabaseName);
    m_sDatabaseUser = parseConfig.stringValue("dbuser", m_sDatabaseUser);
    m_sDatabasePass = parseConfig.stringValue("dbpass", m_sDatabasePass);

    WSJCppLog::info(TAG, "Database host: " + m_sDatabaseHost);
    WSJCppLog::info(TAG, "Database port: " + std::to_string(m_nDatabasePort));
    WSJCppLog::info(TAG, "Database name: " + m_sDatabaseName);
    WSJCppLog::info(TAG, "Database user: " + m_sDatabaseUser);
    WSJCppLog::info(TAG, "Database password: (hided)");

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
        WSJCppLog::err(TAG, "Connect error: " + std::string(mysql_error(pDatabase)));
        WSJCppLog::err(TAG, "Failed to connect.");
    } else {
        pConn = new MySqlStorageConnection(pDatabase, this);
    }
    return pConn;
}

// ----------------------------------------------------------------------

void MySqlStorage::clean() {

}

// ----------------------------------------------------------------------

/*
std::vector<std::string> MySqlStorage::prepareSqlQueries(StorageStruct &storageStruct) {
    std::vector<std::string> vRet;
    if (storageStruct.mode() == StorageStructTableMode::ALTER) {
        // drop columns
        std::vector<std::string> vDropColumns = storageStruct.listDropColumns();
        for (int i = 0; i < vDropColumns.size(); i++) {
            vRet.push_back("ALTER TABLE `" + storageStruct.tableName() + "` DROP COLUMN `" + vDropColumns[i] + "`;");
        }
        
        // add columns
        std::vector<StorageColumnDef> vAddColumns = storageStruct.listAddColumns();
        for (int i = 0; i < vAddColumns.size(); i++) {
            vRet.push_back("ALTER TABLE `" + storageStruct.tableName() + "` ADD COLUMN " + generateLineColumnForSql(vAddColumns[i]) + ";");
        }

        // alter columns
        std::vector<StorageColumnDef> vAlterColumns = storageStruct.listAlterColumns();
        for (int i = 0; i < vAlterColumns.size(); i++) {
            vRet.push_back("ALTER TABLE `" + storageStruct.tableName() + "` MODIFY " + generateLineColumnForSql(vAlterColumns[i]) + ";");
        }
    } else if (storageStruct.mode() == StorageStructTableMode::CREATE) {
        // TODO deprecated (Moved to StorageCreateTable)
        std::string sQuery = "";
        sQuery += "CREATE TABLE IF NOT EXISTS `" + storageStruct.tableName() + "` (\r\n";
        std::vector<std::string> vCreateTableContent;
        std::vector<std::string> vCreateTableContentIndexes;
        std::vector<std::string> vCreateTableContentUniqueIndexes;

        // add columns
        std::vector<StorageColumnDef> vAddColumns = storageStruct.listAddColumns();
        for (int i = 0; i < vAddColumns.size(); i++) {
            StorageColumnDef c = vAddColumns[i];
            vCreateTableContent.push_back(this->generateLineColumnForSql(c));

            // sQuery += "  " + generateLineColumnForSql(c) + ",\r\n";
            if (c.isPrimaryKey()) {
                vCreateTableContentIndexes.push_back("PRIMARY KEY (" + c.columnName() + ")");
            }

            if (c.isEnableIndex()) {
                std::string sIndexLine = "KEY idx_" + c.columnName() + " (" + c.columnName();
                if (c.columnType() == "string" && c.columnTypeSize() > 255) {
                    sIndexLine += "(" + std::to_string(255) + ")";
                }
                sIndexLine += ")";
                vCreateTableContentIndexes.push_back(sIndexLine);
            }

            if (c.isEnableUniqueIndex()) {
                std::string sPrefix = "UNIQUE KEY " + c.nameOfUniqueIndex();
                int nFound = -1;
                for (int i = 0; i < vCreateTableContentIndexes.size(); i++) {
                    if (vCreateTableContentIndexes[i].rfind(sPrefix, 0) == 0) {
                        nFound = i;
                        break;
                    }
                }
                if (nFound == -1) {
                    vCreateTableContentIndexes.push_back("UNIQUE KEY " + c.nameOfUniqueIndex() + " (" + c.columnName());
                } else {
                    vCreateTableContentIndexes[nFound] += "," + c.columnName();
                }
            }
        }
        // close uniq indexes
        for (int i = 0; i < vCreateTableContentIndexes.size(); i++) {
            if (vCreateTableContentIndexes[i].rfind("UNIQUE KEY ", 0) == 0) {
                vCreateTableContentIndexes[i] += ")";
            }
        }

        for (int i = 0; i < vCreateTableContentIndexes.size(); i++) {
            vCreateTableContent.push_back(vCreateTableContentIndexes[i]);
        }
        int nSize = vCreateTableContent.size();
        for (int i = 0; i < nSize; i++) {
            sQuery += "  " + vCreateTableContent[i];
            sQuery += (nSize-1 != i) ? "," : "";
            sQuery += "\r\n";
        }
        sQuery += ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;";
        vRet.push_back(sQuery);
    }
    return vRet;
}
*/
// ----------------------------------------------------------------------

std::vector<std::string> MySqlStorage::prepareSqlQueries(const StorageInsert &storageInsert) {
    std::vector<std::string> vRet;
    std::string sSql = "";
    std::string sValues = "";

    std::vector<StorageColumnValue> values = storageInsert.values();
    for (int i = 0; i < values.size(); i++) {
        StorageColumnValue v = values[i];
        sSql += (sSql.length() > 0 ? ", " : "");
        sSql += v.getColumnName();
        sValues += (sValues.length() > 0 ? ", " : "");
        
        if (v.getColumnType() == StorageColumnType::STRING) {
            sValues += this->prepareStringValue(v.getString());
        } else if (v.getColumnType() == StorageColumnType::DATETIME) {
            sValues += this->prepareStringValue(v.getString());
        } else if (v.getColumnType() == StorageColumnType::NUMBER) {
            sValues += std::to_string(v.getInt());
        } else if (v.getColumnType() == StorageColumnType::DOUBLE_NUMBER) {
            sValues += std::to_string(v.getDouble());
        } else {
            WSJCppLog::err(TAG, "Unknown type " + std::to_string(v.getColumnType()));
        }
    }
    vRet.push_back("INSERT INTO " + storageInsert.getTableName() + "(" + sSql + ") VALUES(" + sValues + ");");
    return vRet;
}

// ----------------------------------------------------------------------

std::vector<std::string> MySqlStorage::prepareSqlQueries(const StorageCreateTable &storageCreateTable) {
    std::vector<std::string> vRet;
    std::string sQuery = "";
    sQuery += "CREATE TABLE IF NOT EXISTS `" + storageCreateTable.getTableName() + "` (\r\n";
    std::vector<std::string> vCreateTableContent;
    std::vector<std::string> vCreateTableContentIndexes;
    std::vector<std::string> vCreateTableContentUniqueIndexes;

    // add columns
    std::vector<StorageColumnDef> vColumns = storageCreateTable.getColumns();
    for (int i = 0; i < vColumns.size(); i++) {
        StorageColumnDef c = vColumns[i];
        vCreateTableContent.push_back(this->generateLineColumnForSql(c));

        // sQuery += "  " + generateLineColumnForSql(c) + ",\r\n";
        if (c.isPrimaryKey()) {
            vCreateTableContentIndexes.push_back("PRIMARY KEY (" + c.columnName() + ")");
        }

        if (c.isEnableIndex()) {
            std::string sIndexLine = "KEY idx_" + c.columnName() + " (" + c.columnName();
            if (c.columnType() == "string" && c.columnTypeSize() > 255) {
                sIndexLine += "(" + std::to_string(255) + ")";
            }
            sIndexLine += ")";
            vCreateTableContentIndexes.push_back(sIndexLine);
        }

        if (c.isEnableUniqueIndex()) {
            std::string sPrefix = "UNIQUE KEY " + c.nameOfUniqueIndex();
            int nFound = -1;
            for (int i = 0; i < vCreateTableContentIndexes.size(); i++) {
                if (vCreateTableContentIndexes[i].rfind(sPrefix, 0) == 0) {
                    nFound = i;
                    break;
                }
            }
            if (nFound == -1) {
                vCreateTableContentIndexes.push_back("UNIQUE KEY " + c.nameOfUniqueIndex() + " (" + c.columnName());
            } else {
                vCreateTableContentIndexes[nFound] += "," + c.columnName();
            }
        }
    }
    // close uniq indexes
    for (int i = 0; i < vCreateTableContentIndexes.size(); i++) {
        if (vCreateTableContentIndexes[i].rfind("UNIQUE KEY ", 0) == 0) {
            vCreateTableContentIndexes[i] += ")";
        }
    }

    for (int i = 0; i < vCreateTableContentIndexes.size(); i++) {
        vCreateTableContent.push_back(vCreateTableContentIndexes[i]);
    }

    int nSize = vCreateTableContent.size();
    for (int i = 0; i < nSize; i++) {
        sQuery += "  " + vCreateTableContent[i];
        sQuery += (nSize-1 != i) ? "," : "";
        sQuery += "\r\n";
    }
    sQuery += ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;";
    vRet.push_back(sQuery);
    return vRet;
}

// ----------------------------------------------------------------------

std::vector<std::string> MySqlStorage::prepareSqlQueries(const StorageModifyTable &storageModifyTable) {
    std::vector<std::string> vRet;

    // drop columns
    std::vector<std::string> vDropColumns = storageModifyTable.getDropColumns();
    for (int i = 0; i < vDropColumns.size(); i++) {
        vRet.push_back("ALTER TABLE `" + storageModifyTable.getTableName() + "` DROP COLUMN `" + vDropColumns[i] + "`;");
    }

    // add columns
    std::vector<StorageColumnDef> vAddColumns = storageModifyTable.getAddColumns();
    for (int i = 0; i < vAddColumns.size(); i++) {
        vRet.push_back("ALTER TABLE `" + storageModifyTable.getTableName() + "` ADD COLUMN " + generateLineColumnForSql(vAddColumns[i]) + ";");
    }

    // alter columns
    std::vector<StorageColumnDef> vAlterColumns = storageModifyTable.getAlterColumns();
    for (int i = 0; i < vAlterColumns.size(); i++) {
        vRet.push_back("ALTER TABLE `" + storageModifyTable.getTableName() + "` MODIFY " + generateLineColumnForSql(vAlterColumns[i]) + ";");
    }
    return vRet;
}

// ----------------------------------------------------------------------

std::vector<std::string> MySqlStorage::prepareSqlQueries(const StorageDropTable &storageDropTable) {
    if (!this->existsTable(storageDropTable.getTableName())) {
        WSJCppLog::throw_err(TAG, "Table '" + storageDropTable.getTableName() + "' does not define previously");
    }
    std::vector<std::string> vRet;
    vRet.push_back("DROP TABLE IF EXISTS `" + storageDropTable.getTableName() + "`;");
    return vRet;
}

// ----------------------------------------------------------------------

std::string MySqlStorage::prepareStringValue(const std::string &sValue) {
    // escaping simbols  NUL (ASCII 0), \n, \r, \, ', ", и Control-Z.
    std::string sResult;
    sResult.reserve(sValue.size()*2);
    sResult.push_back('"');
    for (int i = 0; i < sValue.size(); i++) {
        char c = sValue[i];
        if (c == '\n') {
            sResult.push_back('\\');
            sResult.push_back('n');
        } else if (c == '\r') {
            sResult.push_back('\\');
            sResult.push_back('r');
        } else if (c == '\\' || c == '"' || c == '\'') {
            sResult.push_back('\\');
            sResult.push_back(c);
        } else if (c == 0) {
            sResult.push_back('\\');
            sResult.push_back('0');
        } else {
            sResult.push_back(c);
        }
    }
    sResult.push_back('"');
    return sResult;
}

// ----------------------------------------------------------------------

std::string MySqlStorage::generateLineColumnForSql(StorageColumnDef &c) {
    std::string sSqlColumn = "";

    sSqlColumn += "`" + c.columnName() + "`";
    
    if (c.columnType() == "number") {
        sSqlColumn += " INT";
    } else if (c.columnType() == "string") {
        sSqlColumn += " VARCHAR(" + std::to_string(c.columnTypeSize()) + ")";
    } else if (c.columnType() == "text") {
        sSqlColumn += " TEXT";
    } else if (c.columnType() == "datetime") {
        sSqlColumn += " DATETIME";
    } else if (c.columnType() == "doubleNumber") {
        sSqlColumn += " DOUBLE";
    } else {
        WSJCppLog::err(TAG, "Unknown columnType " + c.columnType());
    }

    if (c.isNotNull()) {
        sSqlColumn += " NOT NULL";
    }

    if (c.isAutoIncrement()) {
        sSqlColumn += " AUTO_INCREMENT";
    }

    if (c.columnDefaultValue() != "") {
        sSqlColumn += " DEFAULT " + c.columnDefaultValue();
    }

    return sSqlColumn;
}

// ----------------------------------------------------------------------
