#ifndef INTERFACES_IUPDATE_H
#define INTERFACES_IUPDATE_H

#include <QString>
#include <QSqlDatabase>

class IUpdate {
	public:
		virtual QString from_version() = 0;
		virtual QString version() = 0;
		virtual QString name() = 0;
		virtual QString description() = 0;
		virtual void update(QSqlDatabase &db) = 0;
};

#endif // INTERFACES_IUPDATE_H
