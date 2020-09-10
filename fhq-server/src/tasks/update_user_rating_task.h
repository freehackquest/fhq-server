#ifndef UPDATE_USER_RATING_TASK_H
#define UPDATE_USER_RATING_TASK_H

#include <string>
#include <QRunnable>

class UpdateUserRatingTask : public QRunnable {
    
    void run();

    public:
        UpdateUserRatingTask(int nUserID);
        ~UpdateUserRatingTask();
    private:
        int m_nUserID;
        std::string TAG;
};

#endif // UPDATE_USER_RATING_TASK_H
