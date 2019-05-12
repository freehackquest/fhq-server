#ifndef NOTIFY_TO_ALL_TASK_H
#define NOTIFY_TO_ALL_TASK_H

#include <QRunnable>
#include <json.hpp>

class NotifyToAllTask : public QRunnable {

    void run();

    public:
        NotifyToAllTask(const nlohmann::json &jsonMessage);
        virtual ~NotifyToAllTask();
    private:
        std::string TAG;
        nlohmann::json m_jsonMessage;
};

#endif // NOTIFY_TO_ALL_TASK_H

