#ifndef CREATE_LIST_UPDATES_H
#define CREATE_LIST_UPDATES_H

#include <iupdate.h>
#include <QVector>

void create_list_updates(QVector<IUpdate *> &vUpdates);
bool tryUpdateDatabase(QSqlDatabase *pDatabase);

#endif // CREATE_LIST_UPDATES_H
