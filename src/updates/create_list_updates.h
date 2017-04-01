#ifndef CREATE_LIST_UPDATES_H
#define CREATE_LIST_UPDATES_H

#include "../interfaces/iupdate.h"
#include <QVector>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

void create_list_updates(QVector<IUpdate *> &vUpdates);
void tryUpdateDatabase(QSqlDatabase *pDatabase);
	
#endif // CREATE_LIST_UPDATES_H
