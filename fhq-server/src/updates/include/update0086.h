#ifndef UPDATE_0086_H
#define UPDATE_0086_H

#include <iupdate.h>

class Update0086 : public IUpdate {
	
	public:
        Update0086();
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0086_H
