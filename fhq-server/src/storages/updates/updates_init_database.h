#ifndef UPDATES_INIT_DATABASE_H
#define UPDATES_INIT_DATABASE_H

#include <storages.h>

class UpdatesInitDatabase : public StorageUpdateBase {
    public:
        UpdatesInitDatabase();
        virtual bool custom(Storage *pStorage, StorageConnection *pConn, std::string &error);
};

#endif // UPDATES_INIT_DATABASE_H
