#ifndef SERVER_DATABASE_CONNECTION_H
#define SERVER_DATABASE_CONNECTION_H

#include <iserver.h>

#include <QString>
#include <QJsonObject>
#include <QDateTime>
#include "server_config.h"

class DatabaseConnection {
	public:
		DatabaseConnection(QString sNameConnection);
		void swap(DatabaseConnection *pDatabaseConnection);
		QString nameConnection();
		void setNameConnection(QString sNameConnection);
		bool connect(ServerConfig *pServerConfig);
		QSqlDatabase *db();
		void setDb(QSqlDatabase *pDatabase);
		void close();
		bool isOutdated();
		
	private:
		qint64 m_nOutdatedAfter; // in mseconds
		qint64 m_nOpened;
		QString m_sNameConnection;
		QSqlDatabase *m_pDatabase;
		QString TAG;
};

#endif // SERVER_DATABASE_CONNECTION_H
