#ifndef UPDATE_0071_H
#define UPDATE_0071_H

#include <iupdate.h>

class Update0071 : public IUpdate {
	
    public:
        Update0071();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0071_H
