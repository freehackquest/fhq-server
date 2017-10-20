#ifndef UPDATE0093
#define UPDATE0093

#include <iupdate.h>

class Update0093 : public IUpdate {

    public:
        Update0093();
        virtual QString from_version();
        virtual QString version();
        virtual QString description();
        virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE0093

