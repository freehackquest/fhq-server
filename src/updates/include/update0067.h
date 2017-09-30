#ifndef UPDATE_0067_H
#define UPDATE_0067_H

#include <iupdate.h>

class Update0067 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);
};

#endif // UPDATE_0067_H
