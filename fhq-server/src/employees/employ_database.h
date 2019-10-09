#ifndef EMPLOY_DATABASE_H
#define EMPLOY_DATABASE_H

#include <model_database_connection.h>
#include <wsjcpp_employees.h>

#include <storages.h>
#include <mutex>
#include <QMap>

class EmployDatabase : public WSJCppEmployBase, public WSJCppSettingsStore {
    public:
        EmployDatabase();
        static std::string name() { return "EmployDatabase"; }
        virtual bool init();
        QSqlDatabase *database();
        bool manualCreateDatabase(const std::string& sRootPassword, std::string& sError);
        StorageConnection *getStorageConnection();
        
        // WSJCppSettingsStore
        virtual std::map<std::string, std::string> loadAllSettings();
        virtual void updateSettingItem(const WSJCppSettingItem *pSettingItem);
        virtual void initSettingItem(WSJCppSettingItem *pSettingItem);

    private:
        std::string TAG;
        std::string m_sStorageType;
        int m_nConnectionOutdatedAfterSeconds;
        Storage *m_pStorage;

        // new new
        std::map<std::string, StorageConnection*> m_mapStorageConnections;
        std::vector<StorageConnection*> m_vDoRemoveStorageConnections;
        std::mutex m_mtxStorageConnections;

        // db two connections
        std::mutex m_mtxSwapConenctions;
        

        // old
        QMap<long long, ModelDatabaseConnection *> m_mDatabaseConnections;
        QMap<long long, ModelDatabaseConnection *> m_mDatabaseConnections_older;
        ModelDatabaseConnection *m_pDBConnection;
        ModelDatabaseConnection *m_pDBConnection_older;


};

#endif // EMPLOY_DATABASE_H
