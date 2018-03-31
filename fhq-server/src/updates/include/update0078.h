#ifndef UPDATE_0078_H
#define UPDATE_0078_H

#include <iupdate.h>

class Update0078 : public IUpdate {
	
    public:
        Update0078();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0078_H
