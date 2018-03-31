#ifndef UPDATE_0066_H
#define UPDATE_0066_H

#include <iupdate.h>

class Update0066 : public IUpdate {
	
    public:
        Update0066();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0066_H
