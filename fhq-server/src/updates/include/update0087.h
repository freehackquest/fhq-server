#ifndef UPDATE_0087_H
#define UPDATE_0087_H

#include <iupdate.h>

class Update0087 : public IUpdate {
	
	public:
		Update0087();
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);

	private:
		QString TAG;
};

#endif // UPDATE_0087_H
