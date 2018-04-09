#ifndef UPDATE_USER_RATING_TASK_H
#define UPDATE_USER_RATING_TASK_H

#include <QRunnable>
#include <iserver.h>

class UpdateUserRatingTask : public QRunnable {
	
	void run();

	public:
        UpdateUserRatingTask(int nUserID);
        ~UpdateUserRatingTask();
	private:
        int m_nUserID;
		QString TAG;
		IWebSocketServer *m_pWebSocketServer;
};

#endif // UPDATE_USER_RATING_TASK_H
