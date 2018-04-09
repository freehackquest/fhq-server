#ifndef EMPLOY_DATABASE_H
#define EMPLOY_DATABASE_H

#include <database_connection.h>
#include <employees.h>

#include <QMutex>

class EmployDatabase : public EmployBase {
    public:
       EmployDatabase();
       static std::string name() { return "EmployDatabase"; }
       virtual bool init();
       QSqlDatabase *database();
       
	private:
        // db two connections
		QMutex m_mtxSwapConenctions;
		QMap<long long, DatabaseConnection *> m_mDatabaseConnections;
		QMap<long long, DatabaseConnection *> m_mDatabaseConnections_older;
		DatabaseConnection *m_pDBConnection;
		DatabaseConnection *m_pDBConnection_older;
};

#endif // EMPLOY_DATABASE_H
