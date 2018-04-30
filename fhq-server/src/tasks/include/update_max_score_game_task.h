#ifndef UPDATE_MAX_SCORE_GAME_TASK_H
#define UPDATE_MAX_SCORE_GAME_TASK_H

#include <QRunnable>
#include <iserver.h>

class UpdateMaxScoreGameTask : public QRunnable {
	void run();
	public:
        UpdateMaxScoreGameTask(int gameid);
		~UpdateMaxScoreGameTask();
	private:
		int m_nGameID;
		QString TAG;
		IWebSocketServer *m_pWebSocketServer;
};

#endif // UPDATE_MAX_SCORE_GAME_TASK_H
