#ifndef UPDATE_0076_H
#define UPDATE_0076_H

#include <iupdate.h>

class Update0076 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);
};

#endif // UPDATE_0076_H
