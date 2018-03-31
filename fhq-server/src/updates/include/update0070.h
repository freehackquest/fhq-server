#ifndef UPDATE_0070_H
#define UPDATE_0070_H

#include <iupdate.h>

class Update0070 : public IUpdate {
	
    public:
        Update0070();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0070_H
