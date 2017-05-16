#include "../headers/database_connection.h"
#include <QJsonDocument>
#include <QJsonObject>

DatabaseConnection::DatabaseConnection(QString sNameConnection){
	m_pDatabase = NULL;
	m_sNameConnection = sNameConnection;
	m_nOutdatedAfter = 60*60*1000; // every 1 hour reconnect to database
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

bool DatabaseConnection::connect(ServerConfig *pServerConfig){
	m_pDatabase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", m_sNameConnection));
	m_pDatabase->setHostName(pServerConfig->databaseHost());
	m_pDatabase->setDatabaseName(pServerConfig->databaseName());
	m_pDatabase->setUserName(pServerConfig->databaseUser());
	m_pDatabase->setPassword(pServerConfig->databasePassword());
	if (!m_pDatabase->open()){
		qDebug() << m_pDatabase->lastError().text();
		qDebug() << "Failed to connect.";
		m_pDatabase = NULL;
		return false;
	}
	m_nOpened = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
	qDebug() << "Success connection to database (Opened: " << m_sNameConnection << ")";
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
		qDebug() << "Database is not connected";
		return NULL;
	}
	if(!m_pDatabase->isOpen()){
		qDebug() << "Database is not open";
	}
	
	if(m_pDatabase->isOpenError()){
		qDebug() << "Database connection has error";
	}
	
	if(!m_pDatabase->isValid()){
		qDebug() << "Database connection invalid";
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
		qDebug() << "Closed connection to database (" << m_sNameConnection << ")";
	}
}
