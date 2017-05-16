#ifndef INTERFACES_IMEMORYCACHE_H
#define INTERFACES_IMEMORYCACHE_H

#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class IMemoryCache {
	public:
		virtual QString name() = 0;
};

#endif // INTERFACES_IMEMORYCACHE_H
