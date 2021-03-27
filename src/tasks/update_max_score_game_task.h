#ifndef UPDATE_MAX_SCORE_GAME_TASK_H
#define UPDATE_MAX_SCORE_GAME_TASK_H

#include <QRunnable>
#include <cmd_handlers.h>

class UpdateMaxScoreGameTask : public QRunnable {
    void run();
    public:
        UpdateMaxScoreGameTask(int gameid);
        ~UpdateMaxScoreGameTask();
    private:
        int m_nGameID;
        std::string TAG;
        IWebSocketServer *m_pWebSocketServer;
};

#endif // UPDATE_MAX_SCORE_GAME_TASK_H
