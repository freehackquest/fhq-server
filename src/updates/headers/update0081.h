#ifndef UPDATE_0081_H
#define UPDATE_0081_H

#include <iupdate.h>

class Update0081 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // UPDATE_0081_H
