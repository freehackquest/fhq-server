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
        static void MailSend(IWebSocketServer *pWebSocketServer,  QString to, QString subject, QString content);
        static void NotifyToAll(const nlohmann::json &jsonMessage);
        static void LXDAsyncOperation(void (*func)(std::string, std::string &, int &),
                                      std::string sName, std::string sCMD, ModelRequest *pRequest);
};

#endif // RUNTASKS_H
