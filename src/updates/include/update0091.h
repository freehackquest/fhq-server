#ifndef UPDATE_0091_H
#define UPDATE_0091_H

#include <iupdate.h>

class Update0091 : public IUpdate {

    public:
        Update0091();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE0091_H
