#ifndef UPDATE_USER_LOCATION_TASK_H
#define UPDATE_USER_LOCATION_TASK_H

#include <QRunnable>
#include <iserver.h>

class UpdateUserLocationTask : public QRunnable {
	
	void run();
	public:
		UpdateUserLocationTask(IWebSocketServer *pWebSocketServer, int userid, QString lastip);
		~UpdateUserLocationTask();
	private:
		int m_nUserID;
		QString mLastIP;
		IWebSocketServer *m_pWebSocketServer;
		QString TAG;
		
};

#endif // UPDATE_USER_LOCATION_TASK_H
