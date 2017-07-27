#ifndef UPDATE_0086_H
#define UPDATE_0086_H

#include <iupdate.h>

class Update0086 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // UPDATE_0086_H
