#ifndef MYSQL_STORAGE_H
#define MYSQL_STORAGE_H

#include <storages.h>
#include <map>
#include <mysql/mysql.h>
#include <mutex>

class PostgreSqlStorageConnection : public StorageConnection {
    public:
        PostgreSqlStorageConnection(MYSQL *pConn, Storage *pStorage);
        virtual ~PostgreSqlStorageConnection();
        virtual bool executeQuery(const std::string &sQuery);
        virtual std::string lastDatabaseVersion();
        virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription);
    private:
        MYSQL *m_pConnection;
        std::mutex m_mtxConn;
        Storage *m_pStorage;
};

class PostgreSqlStorage : public Storage {
    public:
        PostgreSqlStorage();
        static std::string type() { return "mysql"; };
        virtual bool applyConfigFromFile(const std::string &sFilePath);
        virtual StorageConnection *connect();
        virtual void clean();
        virtual std::vector<std::string> prepareSqlQueries(StorageStruct &storageStruct);
        virtual std::vector<std::string> prepareSqlQueries(const StorageInsert &storageInsert);
        virtual std::string prepareStringValue(const std::string &sValue);

    private:
        std::string generateLineColumnForSql(StorageStructColumn &c);
        std::string TAG;
        std::string m_sDatabaseHost;
        std::string m_sDatabaseName;
        std::string m_sDatabaseUser;
        std::string m_sDatabasePass;
        int m_nDatabasePort;
};

#endif // MYSQL_STORAGE_H
