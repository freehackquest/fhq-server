#include <model_database_connection.h>
#include <employ_server_config.h>
#include <utils_logger.h>

ModelDatabaseConnection::ModelDatabaseConnection(QString sNameConnection){
    m_pDatabase = NULL;
    m_sNameConnection = sNameConnection;
    m_nOutdatedAfter = 60*60*1000; // every 1 hour reconnect to database
    TAG = "ModelDatabaseConnection";
};

// ---------------------------------------------------------------------

void ModelDatabaseConnection::swap(ModelDatabaseConnection *pDatabaseConnection){
    QSqlDatabase *pDatabase = m_pDatabase;
    m_pDatabase = pDatabaseConnection->db();
    pDatabaseConnection->setDb(pDatabase);
    
    QString sNameConnection = m_sNameConnection;
    m_sNameConnection = pDatabaseConnection->nameConnection();
    pDatabaseConnection->setNameConnection(sNameConnection);
}

// ---------------------------------------------------------------------

QString ModelDatabaseConnection::nameConnection(){
    return m_sNameConnection;
}

// ---------------------------------------------------------------------

void ModelDatabaseConnection::setNameConnection(QString sNameConnection){
    m_sNameConnection = sNameConnection;
}

// ---------------------------------------------------------------------

bool ModelDatabaseConnection::connect(){
    EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
    
    m_pDatabase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", m_sNameConnection));
    m_pDatabase->setHostName(QString(pServerConfig->databaseHost().c_str()));
    m_pDatabase->setDatabaseName(QString(pServerConfig->databaseName().c_str()));
    m_pDatabase->setPort(pServerConfig->databasePort());
    m_pDatabase->setUserName(QString(pServerConfig->databaseUser().c_str()));
    m_pDatabase->setPassword(QString(pServerConfig->databasePassword().c_str()));
    if (!m_pDatabase->open()){
        Log::err(TAG, "Failed to connect." + m_pDatabase->lastError().text().toStdString());
        m_pDatabase = NULL;
        return false;
    }
    m_nOpened = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    Log::info(TAG, "Success connection to database (Opened: " + m_sNameConnection.toStdString() + ")");
    return true;
}

// ---------------------------------------------------------------------

bool ModelDatabaseConnection::isOutdated(){
    qint64 nCurrent = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    return m_nOpened + m_nOutdatedAfter < nCurrent;
}

// ---------------------------------------------------------------------

QSqlDatabase *ModelDatabaseConnection::db(){
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

void ModelDatabaseConnection::setDb(QSqlDatabase *pDatabase){
    m_pDatabase = pDatabase;
}

// ---------------------------------------------------------------------

void ModelDatabaseConnection::close(){
    if(m_pDatabase != NULL){
        if (m_pDatabase->isOpen()){
            m_pDatabase->close();
        }
        delete m_pDatabase;
        m_pDatabase = NULL;
        QSqlDatabase::removeDatabase(m_sNameConnection);
        Log::info(TAG, "Closed connection to database (" + m_sNameConnection.toStdString() + ")");
    }
}
