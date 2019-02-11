#ifndef MYSQL_STORAGE_H
#define MYSQL_STORAGE_H

#include <storages.h>
#include <map>
#include <mysql/mysql.h>

class MySqlStorage : public Storage {
    public:
        MySqlStorage();
        static std::string type() { return "mysql"; };
        virtual bool applyConfigFromFile(const std::string &sFilePath);
        virtual bool connect();
        virtual std::string lastVersion();
        virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription);
        virtual void clean();
        virtual std::vector<std::string> prepareSqlQueries(StorageStruct &storageStruct);
        virtual bool applyStruct(StorageStruct &storageStruct);

    private:
        std::string TAG;
        std::string m_sDatabaseHost;
        std::string m_sDatabaseName;
        std::string m_sDatabaseUser;
        std::string m_sDatabasePass;
        int m_nDatabasePort;

        MYSQL *m_pDatabase;
};

#endif // MYSQL_STORAGE_H
