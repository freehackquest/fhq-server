#ifndef UPDATE_0093_H
#define UPDATE_0093_H

#include <updates.h>

class Update0093 : public UpdateBase {

    public:
        Update0093();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0093)

#endif // UPDATE_0093_H

