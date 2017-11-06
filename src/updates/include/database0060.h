#ifndef DATABASE_0060_H
#define DATABASE_0060_H

#include <iupdate.h>

class Database0060 : public IUpdate {
	
    public:
        Database0060();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // DATABASE_0060_H
