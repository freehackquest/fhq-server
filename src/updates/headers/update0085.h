#ifndef UPDATE_0085_H
#define UPDATE_0085_H

#include <iupdate.h>

class Update0085 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // UPDATE_0085_H
