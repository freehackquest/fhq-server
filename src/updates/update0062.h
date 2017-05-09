#ifndef UPDATE_0062_H
#define UPDATE_0062_H

#include "../interfaces/iupdate.h"

class Update0062 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString name();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // UPDATE_0062_H
