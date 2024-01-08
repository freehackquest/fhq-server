/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#include <employees.h>
#include <model_database_connection.h>

ModelDatabaseConnection::ModelDatabaseConnection(QString sNameConnection) {
  m_pDatabase = NULL;
  m_sNameConnection = sNameConnection;
  m_nOutdatedAfter = 60 * 60 * 1000; // every 1 hour reconnect to database
  TAG = "ModelDatabaseConnection";
};

void ModelDatabaseConnection::swap(ModelDatabaseConnection *pDatabaseConnection) {
  QSqlDatabase *pDatabase = m_pDatabase;
  m_pDatabase = pDatabaseConnection->db();
  pDatabaseConnection->setDb(pDatabase);

  QString sNameConnection = m_sNameConnection;
  m_sNameConnection = pDatabaseConnection->nameConnection();
  pDatabaseConnection->setNameConnection(sNameConnection);
}

QString ModelDatabaseConnection::nameConnection() { return m_sNameConnection; }

void ModelDatabaseConnection::setNameConnection(QString sNameConnection) { m_sNameConnection = sNameConnection; }

bool ModelDatabaseConnection::connect() {
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
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
    WsjcppLog::err(TAG, "Failed to connect." + m_pDatabase->lastError().text().toStdString());
    m_pDatabase = NULL;
    return false;
  }
  m_nOpened = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
  WsjcppLog::info(TAG, "Success connection to database (Opened: " + m_sNameConnection.toStdString() + ")");
  return true;
}

bool ModelDatabaseConnection::isOutdated() {
  qint64 nCurrent = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
  return m_nOpened + m_nOutdatedAfter < nCurrent;
}

QSqlDatabase *ModelDatabaseConnection::db() {
  if (m_pDatabase == NULL) {
    WsjcppLog::err(TAG, "Database is not connected");
    return NULL;
  }
  if (!m_pDatabase->isOpen()) {
    WsjcppLog::err(TAG, "Database is not open");
  }

  if (m_pDatabase->isOpenError()) {
    WsjcppLog::err(TAG, "Database connection has error");
  }

  if (!m_pDatabase->isValid()) {
    WsjcppLog::err(TAG, "Database connection invalid");
  }

  return m_pDatabase;
}

void ModelDatabaseConnection::setDb(QSqlDatabase *pDatabase) { m_pDatabase = pDatabase; }

void ModelDatabaseConnection::close() {
  if (m_pDatabase != NULL) {
    if (m_pDatabase->isOpen()) {
      m_pDatabase->close();
    }
    delete m_pDatabase;
    m_pDatabase = NULL;
    QSqlDatabase::removeDatabase(m_sNameConnection);
    WsjcppLog::info(TAG, "Closed connection to database (" + m_sNameConnection.toStdString() + ")");
  }
}
