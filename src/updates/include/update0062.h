#ifndef UPDATE_0062_H
#define UPDATE_0062_H

#include <iupdate.h>

class Update0062 : public IUpdate {
	
    public:
        Update0062();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0062_H
