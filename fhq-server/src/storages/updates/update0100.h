#ifndef UPDATE0100_H
#define UPDATE0100_H

#include <updates.h>

class Update0100 : public UpdateBase {

    public:
        Update0100();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

#endif // UPDATE0100_H

