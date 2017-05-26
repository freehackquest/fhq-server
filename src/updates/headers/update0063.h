#ifndef UPDATE_0063_H
#define UPDATE_0063_H

#include <iupdate.h>

class Update0063 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // UPDATE_0063_H
