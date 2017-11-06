#ifndef UPDATE_0073_H
#define UPDATE_0073_H

#include <iupdate.h>

class Update0073 : public IUpdate {
	
    public:
        Update0073();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0073_H
