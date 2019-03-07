#ifndef UPDATE0100_H
#define UPDATE0100_H

#include <storages.h>

class Update0100 : public StorageUpdateBase {

    public:
        Update0100();
        virtual bool custom(Storage *pStorage, StorageConnection *pConn, std::string &error);
};

#endif // UPDATE0100_H

