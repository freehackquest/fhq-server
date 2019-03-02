#ifndef UPDATE0095
#define UPDATE0095

#include <updates.h>

class Update0095 : public UpdateBase {

    public:
        Update0095();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Update0095)

#endif // UPDATE0095

