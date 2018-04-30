#ifndef MODEL_DATABASE_CONNECTION_H
#define MODEL_DATABASE_CONNECTION_H

#include <QString>
#include <QJsonObject>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>

class ModelDatabaseConnection {
	public:
		ModelDatabaseConnection(QString sNameConnection);
		void swap(ModelDatabaseConnection *pDatabaseConnection);
		QString nameConnection();
		void setNameConnection(QString sNameConnection);
		bool connect();
		QSqlDatabase *db();
		void setDb(QSqlDatabase *pDatabase);
		void close();
		bool isOutdated();
		
	private:
		std::string TAG;
		qint64 m_nOutdatedAfter; // in mseconds
		qint64 m_nOpened;
		QString m_sNameConnection;
		QSqlDatabase *m_pDatabase;
};

#endif // MODEL_DATABASE_CONNECTION_H
