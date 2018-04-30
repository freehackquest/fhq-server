#ifndef RUNTASKS_H
#define RUNTASKS_H

#include <iserver.h>
#include <QString>

class RunTasks {
	public:
        static void AddPublicEvents(QString type, QString message);
        static void UpdateUserLocation(int userid, QString lastip);
        static void UpdateMaxScoreGame(int gameid);
        static void UpdateUserRating(int nUserID);
        static void UpdateQuestSolved(int nQuestID);
        static void MailSend(IWebSocketServer *pWebSocketServer,  QString to, QString subject, QString content);
};

#endif // RUNTASKS_H
