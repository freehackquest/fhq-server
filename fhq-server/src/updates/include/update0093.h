#ifndef UPDATE_0093_H
#define UPDATE_0093_H

#include <iupdate.h>

class Update0093 : public IUpdate {

    public:
        Update0093();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0093_H

