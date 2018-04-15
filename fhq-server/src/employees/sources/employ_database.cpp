#include <employ_server_config.h>
#include <employ_database.h>
#include <updates.h>
#include <QThread>

REGISTRY_EMPLOY(EmployDatabase)

// ---------------------------------------------------------------------

EmployDatabase::EmployDatabase()
    : EmployBase(EmployDatabase::name(), {EmployServerConfig::name()}) {

}

// ---------------------------------------------------------------------

bool EmployDatabase::init(){
	EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
	
	
	m_pDBConnection = new DatabaseConnection("qt_sql_default_connection_1");
	m_pDBConnection_older = new DatabaseConnection("qt_sql_default_connection_2");
	
	if(!m_pDBConnection->connect()){
		return false;
	}

    if(!Updates::updateDatabase(m_pDBConnection->db())){
        return false;
    }

	// TODO: redesign
	// cleanup old user tokens
    /*{
		QSqlDatabase db = *(m_pDBConnection->db());
		QSqlQuery query(db);
		query.prepare("DELETE FROM users_tokens WHERE end_date < NOW()");
		query.exec();
    }*/
	
	
    // TODO
    return true;
}

// ---------------------------------------------------------------------

QSqlDatabase *EmployDatabase::database(){
	// swap connection
	QMutexLocker locker (&m_mtxSwapConenctions);
	
	long long nThreadID = (long long)QThread::currentThreadId();
	
	
	if(m_mDatabaseConnections.contains(nThreadID)){
		DatabaseConnection *pDBConnection = m_mDatabaseConnections[nThreadID];
		DatabaseConnection *pDBConnection_older = m_mDatabaseConnections_older[nThreadID];
		
		if(pDBConnection->isOutdated()){
			pDBConnection_older->close();
			pDBConnection_older->swap(pDBConnection);
			pDBConnection->connect();
		}
		return pDBConnection->db();
	}

	DatabaseConnection *pDBConnection = new DatabaseConnection("qt_sql_default_connection_1_" + QString::number(nThreadID));
	DatabaseConnection *pDBConnection_older = new DatabaseConnection("qt_sql_default_connection_2_" + QString::number(nThreadID));
	m_mDatabaseConnections[nThreadID] = pDBConnection;
	m_mDatabaseConnections_older[nThreadID] = pDBConnection_older;
	pDBConnection->connect();
	return pDBConnection->db();
}
