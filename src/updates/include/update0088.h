#ifndef UPDATE_0088_H
#define UPDATE_0088_H

#include <iupdate.h>

class Update0088 : public IUpdate {
	
	public:
		Update0088();
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);

	private:
		QString TAG;
};

#endif // UPDATE_0088_H
