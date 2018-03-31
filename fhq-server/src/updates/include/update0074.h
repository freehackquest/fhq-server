#ifndef UPDATE_0074_H
#define UPDATE_0074_H

#include <iupdate.h>

class Update0074 : public IUpdate {
	
    public:
        Update0074();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0074_H
