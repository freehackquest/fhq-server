#ifndef UPDATE_0070_H
#define UPDATE_0070_H

#include <iupdate.h>

class Update0070 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);
};

#endif // UPDATE_0070_H
