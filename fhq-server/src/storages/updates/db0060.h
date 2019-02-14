#ifndef STORAGES_DATABASE_0060_H
#define STORAGES_DATABASE_0060_H

#include <storages.h>

class StorageStruct0060 : public StorageUpdateBase {
	
    public:
        StorageStruct0060();
        virtual bool apply(Storage *pStorage, std::string &error);
};

REGISTRY_STORAGE_UPDATE(StorageStruct0060)

#endif // STORAGES_DATABASE_0060_H
