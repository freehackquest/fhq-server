#ifndef UPDATE_0094_H
#define UPDATE_0094_H

#include <iupdate.h>

class Update0094 : public IUpdate {

    public:
        Update0094();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0094_H

