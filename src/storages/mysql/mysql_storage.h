#ifndef MYSQL_STORAGE_H
#define MYSQL_STORAGE_H

#include <wsjcpp_storages.h>
#include <map>
#include <mysql/mysql.h>
#include <mutex>

class MySqlStorageConnection : public WsjcppStorageConnection {
    public:
        MySqlStorageConnection(MYSQL *pConn, WsjcppStorage *pStorage);
        virtual ~MySqlStorageConnection();
        virtual bool executeQuery(const std::string &sQuery);
        virtual std::string lastDatabaseVersion();
        virtual std::vector<std::string> getInstalledVersions() override;
        virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription);
    private:
        MYSQL *m_pConnection;
        std::mutex m_mtxConn;
        WsjcppStorage *m_pStorage;
};

class MySqlStorage : public WsjcppStorage {
    public:
        MySqlStorage();
        static std::string type() { return "mysql"; };
        virtual bool applyConfigFromFile(const std::string &sFilePath);
        virtual WsjcppStorageConnection *connect();
        virtual void clean();
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageInsert &storageInsert);
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageCreateTable &storageCreateTable);
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageModifyTable &storageModifyTable);
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageDropTable &storageDropTable);
        virtual std::string prepareStringValue(const std::string &sValue);
        
    private:
        std::string generateLineColumnForSql(WsjcppStorageColumnDef &c);
        std::string TAG;
        std::string m_sDatabaseHost;
        std::string m_sDatabaseName;
        std::string m_sDatabaseUser;
        std::string m_sDatabasePass;
        int m_nDatabasePort;
};

#endif // MYSQL_STORAGE_H
