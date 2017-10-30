#ifndef UPDATE_0085_H
#define UPDATE_0085_H

#include <iupdate.h>

class Update0085 : public IUpdate {
	
	public:
        Update0085();
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);

    private:
        QString TAG;
};

#endif // UPDATE_0085_H
