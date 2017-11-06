#ifndef UPDATE_0077_H
#define UPDATE_0077_H

#include <iupdate.h>

class Update0077 : public IUpdate {
	
    public:
        Update0077();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0077_H
