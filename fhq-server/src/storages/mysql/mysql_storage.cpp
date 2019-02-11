#include <mysql_storage.h>
#include <utils_logger.h>
#include <mysql/mysql.h>
// #include <conf_file_parser.h>
// #include <fs.h>
// #include <ts.h>

REGISTRY_STORAGE(MySqlStorage)

MySqlStorage::MySqlStorage() {
    TAG = "MySqlStorage";
    m_sDatabaseHost = "";
    m_sDatabaseName = "";
    m_sDatabaseUser = "";
    m_sDatabasePass = "";
    m_nDatabasePort = 3306;
    m_pDatabase = nullptr;
}

// ----------------------------------------------------------------------

bool MySqlStorage::applyConfigFromFile(const std::string &sFilePath) {
    // TODO
    return false;
}

// ----------------------------------------------------------------------

bool MySqlStorage::connect() {
    m_pDatabase = mysql_init(NULL);
    if (!mysql_real_connect(m_pDatabase, 
            m_sDatabaseHost.c_str(),
            m_sDatabaseUser.c_str(),
            m_sDatabasePass.c_str(),
            m_sDatabaseName.c_str(), 
            m_nDatabasePort, NULL, 0)) {
        Log::err(TAG, std::string(mysql_error(m_pDatabase)));
        Log::err(TAG, "Failed to connect.");
        m_pDatabase = nullptr;
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------

std::string MySqlStorage::lastVersion() {

    /*QSqlDatabase db = *pDatabase;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM updates ORDER BY id DESC LIMIT 0,1");
    query.exec();
    std::string sLastVersion = "";
    if (query.next()) {
        QSqlRecord record = query.record();
        // int updateid = record.value("id").toInt();
        sLastVersion = record.value("version").toString().toStdString();
        // std::string description = record.value("description").toString().toStdString();
    }*/
    return "";
}

// ----------------------------------------------------------------------

bool MySqlStorage::insertUpdateInfo(const std::string &sVersion, const std::string &sDescription) {
     /*QSqlQuery insert_query(db);
        insert_query.prepare("INSERT INTO updates (version, description, datetime_update) "
                "VALUES (:version, :description, NOW())");
        insert_query.bindValue(":version", QString(pUpdate->version().c_str()));
        insert_query.bindValue(":description", QString(pUpdate->description().c_str()));
        if(!insert_query.exec()){
            Log::err(TAG, "Could not insert row to updates: " + insert_query.lastError().text().toStdString());
            return false;
        }*/
    return false;
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
            vRet.push_back("ALTER TABLE " + storageStruct.tableName() + " ADD COLUMN " + vAddColumns[i].columnName());
        }

        // alter columns
        std::vector<StorageStructColumn> vAlterColumns = storageStruct.listAlterColumns();
        for (int i = 0; i < vAlterColumns.size(); i++) {
            vRet.push_back("ALTER TABLE " + storageStruct.tableName() + " MODIFY " + vAddColumns[i].columnName());
        }
    } else if (storageStruct.mode() == StorageStructTableMode::DROP) {
        vRet.push_back("DROP TABLE IF EXISTS " + storageStruct.tableName() + ";");
    } else if (storageStruct.mode() == StorageStructTableMode::CREATE) {
        std::string sQuery = "";
        sQuery += "CREATE TABLE IF NOT EXISTS " + storageStruct.tableName() + " (";
        std::string sPrimaryKeyColumnName = "";
        // add columns
        std::vector<StorageStructColumn> vAddColumns = storageStruct.listAddColumns();
        for (int i = 0; i < vAddColumns.size(); i++) {
            StorageStructColumn c = vAddColumns[i];
            sQuery += "  " + c.columnName() + " ,";
            if (c.isPrimaryKey()) {
                sPrimaryKeyColumnName = c.columnName();
            }
        }
        sQuery += " PRIMARY KEY (" + sPrimaryKeyColumnName + ")";
        sQuery += ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;";
        vRet.push_back(sQuery);
    }
    return vRet;
}

// ----------------------------------------------------------------------

bool MySqlStorage::applyStruct(StorageStruct &storageStruct) {
    // TODO make from struct to sql query/queries
    return false;
}

// ----------------------------------------------------------------------

