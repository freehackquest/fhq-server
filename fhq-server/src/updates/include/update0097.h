#ifndef UPDATE0097_H
#define UPDATE0097_H

#include <updates.h>

class Update0097 : public UpdateBase {

    public:
        Update0097();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0097)

#endif // UPDATE0097_H

