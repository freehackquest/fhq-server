#ifndef UPDATE_0082_H
#define UPDATE_0082_H

#include <iupdate.h>

class Update0082 : public IUpdate {
	
    public:
        Update0082();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0082_H
