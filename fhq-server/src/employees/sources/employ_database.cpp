#include <employ_server_config.h>
#include <employ_database.h>
#include <updates.h>
#include <QThread>

REGISTRY_EMPLOY(EmployDatabase)

// ---------------------------------------------------------------------

EmployDatabase::EmployDatabase()
    : EmployBase(EmployDatabase::name(), {EmployServerConfig::name()}) {
	TAG = EmployDatabase::name();
}

// ---------------------------------------------------------------------

bool EmployDatabase::init(){
	// EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
	
	
	m_pDBConnection = new ModelDatabaseConnection("qt_sql_default_connection_1");
	m_pDBConnection_older = new ModelDatabaseConnection("qt_sql_default_connection_2");
	
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

bool EmployDatabase::manualCreateDatabase(const std::string& sRootPassword, std::string& sError){
	EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
	
	QSqlDatabase *pDatabase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "manualCreateDatabase"));
	
	pDatabase->setHostName(QString(pServerConfig->databaseHost().c_str()));
    pDatabase->setPort(pServerConfig->databasePort());
    pDatabase->setUserName("root");
    pDatabase->setPassword(QString(sRootPassword.c_str()));
    if(!pDatabase->open()){
		sError = "Could not connect to mysql://" + pServerConfig->databaseHost() + ":" + std::to_string(pServerConfig->databasePort());
		Log::err(TAG, sError);
		Log::err(TAG, "Maybe wrong root password");
		return false;
	}
    Log::info(TAG, "Success connected");

	std::string dbname = pServerConfig->databaseName();
	std::string dbuser = pServerConfig->databaseUser();
	std::string dbpass = pServerConfig->databasePassword();
	
	// check the database exists
	bool bDatabaseAlreadyExists = false;
	{    
		QSqlQuery query(*pDatabase);
		query.prepare("SHOW DATABASES;");
		if(!query.exec()){
			sError = query.lastError().text().toStdString();
			Log::err(TAG, sError);
			return false;
		}
		while (query.next()) {
			QSqlRecord record = query.record();
			std::string sDatabaseName = record.value("Database").toString().toStdString();
			if(sDatabaseName == dbname){
				bDatabaseAlreadyExists = true;
			}
		}
	}
    
    // check that user exists
    bool bUserAlreadyExists = false;
    {    
		QSqlQuery query(*pDatabase);
		// TODO escaping
		std::string sQuery = "select user from mysql.user where user = '" + dbuser + "';";
		query.prepare(QString(sQuery.c_str()));
		if(!query.exec()){
			sError = query.lastError().text().toStdString();
			Log::err(TAG, sError);
			return false;
		}
		while (query.next()) {
			QSqlRecord record = query.record();
			std::string sUserName = record.value("user").toString().toStdString();
			if(sUserName == dbuser){
				bUserAlreadyExists = true;
			}
		}
	}
    
    if(bDatabaseAlreadyExists || bUserAlreadyExists){
		sError = "";
		if(bDatabaseAlreadyExists){
			sError += "Database '" + dbname + "' already exists. If you wish: you can drop database manually and try again\n";
		}
		if(bUserAlreadyExists){
			sError += "User '" + dbuser + "' already exists. If you wish: you can drop user manually and try again\n";
		}
		return false;
	}
	
	// try create database
	{
		QSqlQuery query(*pDatabase);
		// TODO escaping
		std::string sQuery = "CREATE DATABASE `" + dbname + "` CHARACTER SET utf8 COLLATE utf8_general_ci;";
		query.prepare(QString(sQuery.c_str()));
		if(!query.exec()){
			sError = query.lastError().text().toStdString();
			Log::err(TAG, sError);
			return false;
		}
	}
	
	// try create user
	{
		QSqlQuery query(*pDatabase);
		// TODO escaping
		std::string sQuery = "CREATE USER '" + dbuser + "'@'localhost' IDENTIFIED BY '" + dbpass + "';";
		query.prepare(QString(sQuery.c_str()));
		if(!query.exec()){
			sError = query.lastError().text().toStdString();
			Log::err(TAG, sError);
			return false;
		}
	}
	
	// apply priveleges
	{
		QSqlQuery query(*pDatabase);
		// TODO escaping
		std::string sQuery = "GRANT ALL PRIVILEGES ON " + dbname + ".* TO '" + dbuser + "'@'localhost' WITH GRANT OPTION;";
		query.prepare(QString(sQuery.c_str()));
		if(!query.exec()){
			sError = query.lastError().text().toStdString();
			Log::err(TAG, sError);
			return false;
		}
	}
	
    return true;
}

// ---------------------------------------------------------------------

QSqlDatabase *EmployDatabase::database(){
	// swap connection
	QMutexLocker locker (&m_mtxSwapConenctions);
	
	long long nThreadID = (long long)QThread::currentThreadId();
	
	
	if(m_mDatabaseConnections.contains(nThreadID)){
		ModelDatabaseConnection *pDBConnection = m_mDatabaseConnections[nThreadID];
		ModelDatabaseConnection *pDBConnection_older = m_mDatabaseConnections_older[nThreadID];
		
		if(pDBConnection->isOutdated()){
			pDBConnection_older->close();
			pDBConnection_older->swap(pDBConnection);
			pDBConnection->connect();
		}
		return pDBConnection->db();
	}

	ModelDatabaseConnection *pDBConnection = new ModelDatabaseConnection("qt_sql_default_connection_1_" + QString::number(nThreadID));
	ModelDatabaseConnection *pDBConnection_older = new ModelDatabaseConnection("qt_sql_default_connection_2_" + QString::number(nThreadID));
	m_mDatabaseConnections[nThreadID] = pDBConnection;
	m_mDatabaseConnections_older[nThreadID] = pDBConnection_older;
	pDBConnection->connect();
	return pDBConnection->db();
}
