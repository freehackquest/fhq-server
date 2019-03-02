#ifndef UPDATE_0092_H
#define UPDATE_0092_H

#include <updates.h>

class Update0092 : public UpdateBase {

    public:
        Update0092();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0092)

#endif // UPDATE_0092_H
