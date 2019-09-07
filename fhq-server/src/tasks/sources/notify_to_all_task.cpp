#include <notify_to_all_task.h>
#include <wjscpp_employees.h>
#include <fallen.h>

NotifyToAllTask::NotifyToAllTask(const nlohmann::json &jsonMessage) {
    TAG = "NotifyToAllTask";
    m_jsonMessage = jsonMessage;
}

NotifyToAllTask::~NotifyToAllTask() {

}

void NotifyToAllTask::run() {
    Log::info(TAG, "send notify");
    EmployServer *pServer = findEmploy<EmployServer>();
    pServer->sendToAll(m_jsonMessage);
}
