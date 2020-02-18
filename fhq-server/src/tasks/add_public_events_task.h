#ifndef ADD_PUBLIC_EVENTS_TASK_H
#define ADD_PUBLIC_EVENTS_TASK_H

#include <QRunnable>
#include <string>

class AddPublicEventsTask : public QRunnable {
        void run();
    public:
        AddPublicEventsTask(const std::string &sType, const std::string &sMessage);
        ~AddPublicEventsTask();
    private:
        std::string m_sMessage;
        std::string m_sType;
        std::string TAG;
};

#endif // ADD_PUBLIC_EVENTS_TASK_H
