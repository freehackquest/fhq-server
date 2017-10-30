#ifndef UPDATE_0079_H
#define UPDATE_0079_H

#include <iupdate.h>

class Update0079 : public IUpdate {
	
    public:
        Update0079();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0079_H
