#include <database_connection.h>
#include <employ_server_config.h>
#include <log.h>


DatabaseConnection::DatabaseConnection(QString sNameConnection){
	m_pDatabase = NULL;
	m_sNameConnection = sNameConnection;
	m_nOutdatedAfter = 60*60*1000; // every 1 hour reconnect to database
	TAG = "DatabaseConnection";
};

// ---------------------------------------------------------------------

void DatabaseConnection::swap(DatabaseConnection *pDatabaseConnection){
	QSqlDatabase *pDatabase = m_pDatabase;
	m_pDatabase = pDatabaseConnection->db();
	pDatabaseConnection->setDb(pDatabase);
	
	QString sNameConnection = m_sNameConnection;
	m_sNameConnection = pDatabaseConnection->nameConnection();
	pDatabaseConnection->setNameConnection(sNameConnection);
}

// ---------------------------------------------------------------------

QString DatabaseConnection::nameConnection(){
	return m_sNameConnection;
}

// ---------------------------------------------------------------------

void DatabaseConnection::setNameConnection(QString sNameConnection){
	m_sNameConnection = sNameConnection;
}

// ---------------------------------------------------------------------

bool DatabaseConnection::connect(){
	EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
	
	m_pDatabase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", m_sNameConnection));
	m_pDatabase->setHostName(pServerConfig->databaseHost());
	m_pDatabase->setDatabaseName(pServerConfig->databaseName());
	m_pDatabase->setUserName(pServerConfig->databaseUser());
	m_pDatabase->setPassword(pServerConfig->databasePassword());
	if (!m_pDatabase->open()){
		Log::err(TAG, "Failed to connect." + m_pDatabase->lastError().text());
		m_pDatabase = NULL;
		return false;
	}
	m_nOpened = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
	Log::info(TAG, "Success connection to database (Opened: " + m_sNameConnection + ")");
	return true;
}

// ---------------------------------------------------------------------

bool DatabaseConnection::isOutdated(){
	qint64 nCurrent = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
	return m_nOpened + m_nOutdatedAfter < nCurrent;
}

// ---------------------------------------------------------------------

QSqlDatabase *DatabaseConnection::db(){
	if(m_pDatabase == NULL){
		Log::err(TAG, "Database is not connected");
		return NULL;
	}
	if(!m_pDatabase->isOpen()){
		Log::err(TAG, "Database is not open");
	}
	
	if(m_pDatabase->isOpenError()){
		Log::err(TAG, "Database connection has error");
	}
	
	if(!m_pDatabase->isValid()){
		Log::err(TAG, "Database connection invalid");
	}
	
	return m_pDatabase;
}

// ---------------------------------------------------------------------

void DatabaseConnection::setDb(QSqlDatabase *pDatabase){
	m_pDatabase = pDatabase;
}

// ---------------------------------------------------------------------

void DatabaseConnection::close(){
	if(m_pDatabase != NULL){
		if (m_pDatabase->isOpen()){
			m_pDatabase->close();
		}
		delete m_pDatabase;
		m_pDatabase = NULL;
		QSqlDatabase::removeDatabase(m_sNameConnection);
		Log::info(TAG, "Closed connection to database (" + m_sNameConnection + ")");
	}
}
