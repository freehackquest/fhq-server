#ifndef UPDATE_0092_H
#define UPDATE_0092_H

#include <iupdate.h>

class Update0092 : public IUpdate {

    public:
        Update0092();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0092_H
