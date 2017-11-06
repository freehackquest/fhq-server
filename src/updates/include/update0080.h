#ifndef UPDATE_0080_H
#define UPDATE_0080_H

#include <iupdate.h>

class Update0080 : public IUpdate {
	
    public:
        Update0080();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0080_H
