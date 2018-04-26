#ifndef EMPLOY_DATABASE_H
#define EMPLOY_DATABASE_H

#include <model_database_connection.h>
#include <employees.h>

#include <QMutex>

class EmployDatabase : public EmployBase {
    public:
       EmployDatabase();
       static std::string name() { return "EmployDatabase"; }
       virtual bool init();
       QSqlDatabase *database();
       bool manualCreateDatabase(const std::string& sRootPassword, std::string& sError);
	private:
		std::string TAG;
        // db two connections
		QMutex m_mtxSwapConenctions;
		QMap<long long, ModelDatabaseConnection *> m_mDatabaseConnections;
		QMap<long long, ModelDatabaseConnection *> m_mDatabaseConnections_older;
		ModelDatabaseConnection *m_pDBConnection;
		ModelDatabaseConnection *m_pDBConnection_older;
};

#endif // EMPLOY_DATABASE_H
