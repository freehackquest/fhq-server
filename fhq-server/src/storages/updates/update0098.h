#ifndef UPDATE0098_H
#define UPDATE0098_H

#include <updates.h>

class Update0098 : public UpdateBase {

    public:
        Update0098();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0098)

#endif // UPDATE0098_H

