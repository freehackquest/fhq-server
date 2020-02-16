#ifndef UPDATE_USER_LOCATION_TASK_H
#define UPDATE_USER_LOCATION_TASK_H

#include <QRunnable>
#include <string>

class UpdateUserLocationTask : public QRunnable {
    
    void run();
    public:
        UpdateUserLocationTask(int userid, const std::string &sLastIP);
        ~UpdateUserLocationTask();
    private:
        int m_nUserID;
        std::string m_sLastIP;
        std::string TAG;
        
};

#endif // UPDATE_USER_LOCATION_TASK_H
