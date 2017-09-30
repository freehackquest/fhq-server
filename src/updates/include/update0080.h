#ifndef UPDATE_0080_H
#define UPDATE_0080_H

#include <iupdate.h>

class Update0080 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);
};

#endif // UPDATE_0080_H
