#ifndef UPDATE0095
#define UPDATE0095

#include <iupdate.h>

class Update0095 : public IUpdate {

    public:
        Update0095();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE0095

