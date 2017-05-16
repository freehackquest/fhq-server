#ifndef INTERFACES_ICACHE_H
#define INTERFACES_ICACHE_H

#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class ICache {
	public:
		virtual QString name() = 0;
};

#endif // INTERFACES_ICACHE_H
