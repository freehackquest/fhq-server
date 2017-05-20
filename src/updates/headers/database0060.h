#ifndef DATABASE_0060_H
#define DATABASE_0060_H

#include <iupdate.h>

class Database0060 : public IUpdate {
	
	public:
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual void update(QSqlDatabase &db);
};

#endif // DATABASE_0060_H
