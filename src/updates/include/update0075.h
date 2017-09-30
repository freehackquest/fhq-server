#ifndef UPDATE_0075_H
#define UPDATE_0075_H

#include <iupdate.h>

class Update0075 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);
};

#endif // UPDATE_0075_H
