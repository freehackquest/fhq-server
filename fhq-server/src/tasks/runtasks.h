#ifndef RUNTASKS_H
#define RUNTASKS_H

#include <cmd_handlers.h>

class RunTasks {
    public:
        static void AddPublicEvents(const std::string &sType, const std::string &sMessage);
        static void UpdateUserLocation(int userid, const std::string &sLastip);
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
