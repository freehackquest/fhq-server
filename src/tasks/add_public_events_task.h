#ifndef ADD_PUBLIC_EVENTS_TASK_H
#define ADD_PUBLIC_EVENTS_TASK_H

#include <QRunnable>
#include <json.hpp>
#include <string>

class AddPublicEventsTask : public QRunnable {
  void run();

public:
  AddPublicEventsTask(const std::string &sType, const std::string &sMessage, const nlohmann::json &jsonMeta);
  ~AddPublicEventsTask();

private:
  std::string m_sMessage;
  std::string m_sType;
  nlohmann::json m_jsonMeta;
  std::string TAG;
};

#endif // ADD_PUBLIC_EVENTS_TASK_H
