#ifndef EMPLOY_SERVER_INFO_H
#define EMPLOY_SERVER_INFO_H

#include <employees.h>
#include <QMutex>
#include <QDateTime>

class EmployServerInfo : public EmployBase {
    public:
       EmployServerInfo();
       static std::string name() { return "EmployServerInfo"; }
       virtual bool init();

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

       QDateTime getServerStart();
       nlohmann::json toJson();

   private:

       QMap<std::string, int> m_requestsCounter;
       QMutex m_mtxIncrementRequests;

       QString TAG;
       QDateTime m_dtServerStarted;
       int m_nCountQuests;
       int m_nCountQuestsAttempt;
       int m_nCountQuestsCompleted;
};

#endif // EMPLOY_SERVER_INFO_H
