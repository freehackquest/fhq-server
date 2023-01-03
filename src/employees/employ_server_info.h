#ifndef EMPLOY_SERVER_INFO_H
#define EMPLOY_SERVER_INFO_H

#include <QDateTime>
#include <QMap>
#include <QMutex>
#include <employees.h>

class EmployServerInfo : public WsjcppEmployBase {
public:
  EmployServerInfo();
  static std::string name() { return "EmployServerInfo"; }
  virtual bool init();
  virtual bool deinit() override;

  void incrementRequests(const std::string &cmd);
  void serverStarted();
  int countQuests();
  int countQuestsAttempt();
  int countQuestsCompleted();
  void incrementQuests();
  void decrementQuests();
  void incrementQuestsAttempt();
  void incrementQuestsCompleted();
  void initCounters();
  nlohmann::json developers();

  long getServerStart();
  nlohmann::json toJson();

private:
  QMap<std::string, int> m_requestsCounter;
  QMutex m_mtxIncrementRequests;

  std::string TAG;
  long m_dtServerStarted;
  int m_nCountQuests;
  int m_nCountQuestsAttempt;
  int m_nCountQuestsCompleted;
};

#endif // EMPLOY_SERVER_INFO_H
