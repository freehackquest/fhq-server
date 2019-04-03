#ifndef RUNTASKS_H
#define RUNTASKS_H

#include <iserver.h>
#include <QString>
#include <model_request.h>

class RunTasks {
    public:
        static void AddPublicEvents(QString type, QString message);
        static void UpdateUserLocation(int userid, QString lastip);
        static void UpdateMaxScoreGame(int gameid);
        static void UpdateUserRating(int nUserID);
        static void UpdateQuestSolved(int nQuestID);
        static void MailSend(const std::string &sTo, const std::string &sSubject, const std::string &sContent);
        static void NotifyToAll(const nlohmann::json &jsonMessage);
        static void LXDAsyncOperation(void (*func)(const std::string &, std::string &, int &),
                                      const std::string &sName, const std::string &sCMD, ModelRequest *pRequest);
        static void waitForDone();
};

#endif // RUNTASKS_H
