#include <model_database_connection.h>
#include <wsjcpp_employees.h>
#include <fallen.h>

ModelDatabaseConnection::ModelDatabaseConnection(QString sNameConnection) {
    m_pDatabase = NULL;
    m_sNameConnection = sNameConnection;
    m_nOutdatedAfter = 60*60*1000; // every 1 hour reconnect to database
    TAG = "ModelDatabaseConnection";
};

// ---------------------------------------------------------------------

void ModelDatabaseConnection::swap(ModelDatabaseConnection *pDatabaseConnection) {
    QSqlDatabase *pDatabase = m_pDatabase;
    m_pDatabase = pDatabaseConnection->db();
    pDatabaseConnection->setDb(pDatabase);
    
    QString sNameConnection = m_sNameConnection;
    m_sNameConnection = pDatabaseConnection->nameConnection();
    pDatabaseConnection->setNameConnection(sNameConnection);
}

// ---------------------------------------------------------------------

QString ModelDatabaseConnection::nameConnection() {
    return m_sNameConnection;
}

// ---------------------------------------------------------------------

void ModelDatabaseConnection::setNameConnection(QString sNameConnection) {
    m_sNameConnection = sNameConnection;
}

// ---------------------------------------------------------------------

bool ModelDatabaseConnection::connect() {
    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();
    std::string sDatabaseHost = pGlobalSettings->get("dbhost").getStringValue();
    int nDatabasePort = pGlobalSettings->get("dbport").getNumberValue();
    std::string sDatabaseName = pGlobalSettings->get("dbname").getStringValue();
    std::string sDatabaseUser = pGlobalSettings->get("dbuser").getStringValue();
    std::string sDatabasePassword = pGlobalSettings->get("dbpass").getPasswordValue();

    m_pDatabase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", m_sNameConnection));
    m_pDatabase->setHostName(QString(sDatabaseHost.c_str()));
    m_pDatabase->setDatabaseName(QString(sDatabaseName.c_str()));
    m_pDatabase->setPort(nDatabasePort);
    m_pDatabase->setUserName(QString(sDatabaseUser.c_str()));
    m_pDatabase->setPassword(QString(sDatabasePassword.c_str()));
    if (!m_pDatabase->open()) {
        WSJCppLog::err(TAG, "Failed to connect." + m_pDatabase->lastError().text().toStdString());
        m_pDatabase = NULL;
        return false;
    }
    m_nOpened = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    WSJCppLog::info(TAG, "Success connection to database (Opened: " + m_sNameConnection.toStdString() + ")");
    return true;
}

// ---------------------------------------------------------------------

bool ModelDatabaseConnection::isOutdated() {
    qint64 nCurrent = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    return m_nOpened + m_nOutdatedAfter < nCurrent;
}

// ---------------------------------------------------------------------

QSqlDatabase *ModelDatabaseConnection::db() {
    if (m_pDatabase == NULL) {
        WSJCppLog::err(TAG, "Database is not connected");
        return NULL;
    }
    if (!m_pDatabase->isOpen()) {
        WSJCppLog::err(TAG, "Database is not open");
    }
    
    if (m_pDatabase->isOpenError()) {
        WSJCppLog::err(TAG, "Database connection has error");
    }
    
    if (!m_pDatabase->isValid()) {
        WSJCppLog::err(TAG, "Database connection invalid");
    }
    
    return m_pDatabase;
}

// ---------------------------------------------------------------------

void ModelDatabaseConnection::setDb(QSqlDatabase *pDatabase) {
    m_pDatabase = pDatabase;
}

// ---------------------------------------------------------------------

void ModelDatabaseConnection::close() {
    if (m_pDatabase != NULL) {
        if (m_pDatabase->isOpen()) {
            m_pDatabase->close();
        }
        delete m_pDatabase;
        m_pDatabase = NULL;
        QSqlDatabase::removeDatabase(m_sNameConnection);
        WSJCppLog::info(TAG, "Closed connection to database (" + m_sNameConnection.toStdString() + ")");
    }
}
