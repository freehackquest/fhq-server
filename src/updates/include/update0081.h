#ifndef UPDATE_0081_H
#define UPDATE_0081_H

#include <iupdate.h>

class Update0081 : public IUpdate {
	
    public:
        Update0081();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0081_H
