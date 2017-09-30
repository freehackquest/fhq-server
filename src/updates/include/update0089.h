#ifndef UPDATE_0089_H
#define UPDATE_0089_H

#include <iupdate.h>

class Update0089 : public IUpdate {
	
	public:
		Update0089();
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);

	private:
		QString TAG;
};

#endif // UPDATE_0089_H
