#ifndef UPDATE0099_H
#define UPDATE0099_H

#include <updates.h>

class Update0099 : public UpdateBase {

    public:
        Update0099();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0099)

#endif // UPDATE0098_H

