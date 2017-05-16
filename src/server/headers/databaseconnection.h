#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <iserver.h>

#include <QString>
#include <QJsonObject>

class DatabaseConnection {
	public:
		DatabaseConnection(QSqlDatabase *pDatabase);
		QSqlDatabase *conn();
	private:
		QSqlDatabase *m_pDatabase;
};

#endif // DATABASECONNECTION_H
