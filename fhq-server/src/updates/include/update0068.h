#ifndef UPDATE_0068_H
#define UPDATE_0068_H

#include <iupdate.h>

class Update0068 : public IUpdate {
	
    public:
        Update0068();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0068_H
