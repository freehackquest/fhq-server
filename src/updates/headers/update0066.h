#ifndef UPDATE_0066_H
#define UPDATE_0066_H

#include <iupdate.h>

class Update0066 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // UPDATE_0066_H
