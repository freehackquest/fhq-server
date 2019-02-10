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
}

// ----------------------------------------------------------------------

bool MySqlStorage::applyConfigFromFile(const std::string &sFilePath) {
    // TODO
    return false;
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

bool MySqlStorage::applyStruct(StorageStruct &storageStruct) {
    // TODO make from struct to sql query/queries
    return false;
}