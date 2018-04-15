#ifndef UPDATE0096_H
#define UPDATE0096_H

#include <updates.h>

class Update0096 : public UpdateBase {

    public:
        Update0096();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0096)

#endif // UPDATE0096_H

