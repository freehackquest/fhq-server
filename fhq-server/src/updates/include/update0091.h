#ifndef UPDATE_0091_H
#define UPDATE_0091_H

#include <updates.h>

// ----------- UPDATE 0091 ---------------

class Update0091 : public UpdateBase {

    public:
        Update0091();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0091)

#endif // UPDATE0091_H
