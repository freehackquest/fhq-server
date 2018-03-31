#ifndef UPDATE_0061_H
#define UPDATE_0061_H

#include <iupdate.h>

class Update0061 : public IUpdate {
	
    public:
        Update0061();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0061_H
