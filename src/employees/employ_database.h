#ifndef EMPLOY_DATABASE_H
#define EMPLOY_DATABASE_H

#include <model_database_connection.h>
#include <employees.h>

#include <wsjcpp_storages.h>
#include <mutex>
#include <QMap>

class EmployDatabase : public WsjcppEmployBase, public WsjcppSettingsStore {
    public:
        EmployDatabase();
        static std::string name() { return "EmployDatabase"; }
        virtual bool init();
        virtual bool deinit();
        QSqlDatabase *database();
        bool manualCreateDatabase(const std::string& sRootPassword, std::string& sError);
        WsjcppStorageConnection *getStorageConnection();
        
        // WsjcppSettingsStore
        virtual std::map<std::string, std::string> loadAllSettings();
        virtual void updateSettingItem(const WsjcppSettingItem *pSettingItem);
        virtual void initSettingItem(WsjcppSettingItem *pSettingItem);

    private:
        std::string TAG;
        std::string m_sStorageType;
        int m_nConnectionOutdatedAfterSeconds;
        WsjcppStorage *m_pStorage;

        // new new
        std::map<std::string, WsjcppStorageConnection*> m_mapStorageConnections;
        std::vector<WsjcppStorageConnection*> m_vDoRemoveStorageConnections;
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
