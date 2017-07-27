#ifndef UPDATE_0074_H
#define UPDATE_0074_H

#include <iupdate.h>

class Update0074 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);
};

#endif // UPDATE_0074_H
