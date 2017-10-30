#ifndef UPDATE_0084_H
#define UPDATE_0084_H

#include <iupdate.h>

class Update0084 : public IUpdate {
	
    public:
        Update0084();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0084_H
