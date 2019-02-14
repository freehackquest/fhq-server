#ifndef EMPLOY_DATABASE_H
#define EMPLOY_DATABASE_H

#include <model_database_connection.h>
#include <employees.h>

#include <storages.h>
#include <mutex>

class EmployDatabase : public EmployBase {
    public:
       EmployDatabase();
       static std::string name() { return "EmployDatabase"; }
       virtual bool init();
       QSqlDatabase *database();
       bool manualCreateDatabase(const std::string& sRootPassword, std::string& sError);
       Storage *storage();

	private:
		std::string TAG;
        std::string m_sStorageType;
        Storage *m_pStorage;

        // new new
        std::map<std::string, Storage *> m_mapStorageConnections;

        // db two connections
        std::mutex m_mtxSwapConenctions;

        // old
		QMap<long long, ModelDatabaseConnection *> m_mDatabaseConnections;
		QMap<long long, ModelDatabaseConnection *> m_mDatabaseConnections_older;
		ModelDatabaseConnection *m_pDBConnection;
		ModelDatabaseConnection *m_pDBConnection_older;


};

#endif // EMPLOY_DATABASE_H
