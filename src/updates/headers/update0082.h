#ifndef UPDATE_0082_H
#define UPDATE_0082_H

#include <iupdate.h>

class Update0082 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // UPDATE_0082_H
