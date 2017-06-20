#ifndef UPDATE_0084_H
#define UPDATE_0084_H

#include <iupdate.h>

class Update0084 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // UPDATE_0084_H
