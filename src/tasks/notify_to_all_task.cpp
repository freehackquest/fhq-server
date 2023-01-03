#include <employees.h>
#include <notify_to_all_task.h>

NotifyToAllTask::NotifyToAllTask(const nlohmann::json &jsonMessage) {
  TAG = "NotifyToAllTask";
  m_jsonMessage = jsonMessage;
}

NotifyToAllTask::~NotifyToAllTask() {}

void NotifyToAllTask::run() {
  WsjcppLog::info(TAG, "send notify");
  EmployServer *pServer = findWsjcppEmploy<EmployServer>();
  pServer->sendToAll(m_jsonMessage);
}
