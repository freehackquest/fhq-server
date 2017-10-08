#ifndef UPDATE_0090_H
#define UPDATE_0090_H

#include <iupdate.h>

class Update0090 : public IUpdate {
	
	public:
        Update0090();
		virtual QString from_version();
		virtual QString version();
		virtual QString description();
		virtual bool update(QSqlDatabase &db, QString &error);

	private:
		QString TAG;
};

#endif // UPDATE_0090_H
