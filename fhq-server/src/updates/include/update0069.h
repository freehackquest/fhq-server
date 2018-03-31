#ifndef UPDATE_0069_H
#define UPDATE_0069_H

#include <iupdate.h>

class Update0069 : public IUpdate {
	
    public:
        Update0069();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0069_H
