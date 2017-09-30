#ifndef UPDATE_0064_H
#define UPDATE_0064_H

#include <iupdate.h>

class Update0064 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);
};

#endif // UPDATE_0064_H
