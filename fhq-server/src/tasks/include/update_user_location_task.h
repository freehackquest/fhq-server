#ifndef UPDATE_USER_LOCATION_TASK_H
#define UPDATE_USER_LOCATION_TASK_H

#include <QRunnable>
#include <iserver.h>

class UpdateUserLocationTask : public QRunnable {
	
	void run();
	public:
        UpdateUserLocationTask(int userid, QString lastip);
		~UpdateUserLocationTask();
	private:
		int m_nUserID;
		QString mLastIP;
		QString TAG;
		
};

#endif // UPDATE_USER_LOCATION_TASK_H
