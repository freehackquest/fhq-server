#include <notify_to_all_task.h>
#include <employ_ws_server.h>
#include <utils_logger.h>

NotifyToAllTask::NotifyToAllTask(const nlohmann::json &jsonMessage) {
    TAG = "NotifyToAllTask";
    m_jsonMessage = jsonMessage;
}

NotifyToAllTask::~NotifyToAllTask() {

}

void NotifyToAllTask::run() {
    Log::info(TAG, "send notify");
    EmployWsServer *pWsServer = findEmploy<EmployWsServer>();
    pWsServer->sendToAll(m_jsonMessage);
}
