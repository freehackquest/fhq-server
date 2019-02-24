#ifndef UPDATES_INIT_DATABASE_H
#define UPDATES_INIT_DATABASE_H

#include <storages.h>

class UpdatesInitDatabase : public StorageUpdateBase {
	
    public:
        UpdatesInitDatabase();
        virtual bool apply(Storage *pStorage, std::string &error);
};

REGISTRY_STORAGE_UPDATE(UpdatesInitDatabase)

#endif // UPDATES_INIT_DATABASE_H
