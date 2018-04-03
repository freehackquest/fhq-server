#ifndef UPDATE0097_H
#define UPDATE0097_H

#include <iupdate.h>

class Update0097 : public IUpdate {

    public:
        Update0097();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE0097_H

