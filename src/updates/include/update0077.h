#ifndef UPDATE_0077_H
#define UPDATE_0077_H

#include <iupdate.h>

class Update0077 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);
};

#endif // UPDATE_0077_H
