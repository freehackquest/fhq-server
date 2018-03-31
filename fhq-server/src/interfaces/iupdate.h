#ifndef INTERFACES_IUPDATE_H
#define INTERFACES_IUPDATE_H

#include <log.h>
#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class IUpdate {
	public:
		virtual QString from_version() = 0;
		virtual QString version() = 0;
		virtual QString description() = 0;
		virtual bool update(QSqlDatabase &db, QString &error) = 0;
};

#endif // INTERFACES_IUPDATE_H
