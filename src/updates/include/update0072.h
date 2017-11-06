#ifndef UPDATE_0072_H
#define UPDATE_0072_H

#include <iupdate.h>

class Update0072 : public IUpdate {
	
    public:
        Update0072();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0072_H
