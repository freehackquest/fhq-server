#ifndef MYSQL_STORAGE_H
#define MYSQL_STORAGE_H

#include <core/storages.h>
#include <map>
#include <mysql/mysql.h>
#include <mutex>

class MySqlStorageConnection : public StorageConnection {
    public:
        MySqlStorageConnection(MYSQL *pConn, Storage *pStorage);
        virtual ~MySqlStorageConnection();
        virtual bool executeQuery(const std::string &sQuery);
        virtual std::string lastDatabaseVersion();
        virtual std::vector<std::string> getInstalledVersions() override;
        virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription);
    private:
        MYSQL *m_pConnection;
        std::mutex m_mtxConn;
        Storage *m_pStorage;
};

class MySqlStorage : public Storage {
    public:
        MySqlStorage();
        static std::string type() { return "mysql"; };
        virtual bool applyConfigFromFile(const std::string &sFilePath);
        virtual StorageConnection *connect();
        virtual void clean();
        virtual std::vector<std::string> prepareSqlQueries(const StorageInsert &storageInsert);
        virtual std::vector<std::string> prepareSqlQueries(const StorageCreateTable &storageCreateTable);
        virtual std::vector<std::string> prepareSqlQueries(const StorageModifyTable &storageModifyTable);
        virtual std::vector<std::string> prepareSqlQueries(const StorageDropTable &storageDropTable);
        virtual std::string prepareStringValue(const std::string &sValue);
        
    private:
        std::string generateLineColumnForSql(StorageColumnDef &c);
        std::string TAG;
        std::string m_sDatabaseHost;
        std::string m_sDatabaseName;
        std::string m_sDatabaseUser;
        std::string m_sDatabasePass;
        int m_nDatabasePort;
};

#endif // MYSQL_STORAGE_H
