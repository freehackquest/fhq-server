#ifndef UPDATE0101_H
#define UPDATE0101_H

#include <storages.h>

class Update0101 : public StorageUpdateBase {

    public:
        Update0101();
        virtual bool custom(Storage *pStorage, StorageConnection *pConn, std::string &error);
};

#endif

