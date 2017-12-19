#ifndef UPDATE0096_H
#define UPDATE0096_H

#include <iupdate.h>

class Update0096 : public IUpdate {

    public:
        Update0096();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE0096_H

