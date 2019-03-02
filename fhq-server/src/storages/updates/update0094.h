#ifndef UPDATE_0094_H
#define UPDATE_0094_H

#include <updates.h>

class Update0094 : public UpdateBase {

    public:
        Update0094();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0094)

#endif // UPDATE_0094_H

