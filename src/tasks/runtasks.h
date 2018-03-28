#ifndef RUNTASKS_H
#define RUNTASKS_H

#include <iserver.h>
#include <QString>

class RunTasks {
	public:
		static void AddPublicEvents(IWebSocketServer *pWebSocketServer, QString type, QString message);
		static void UpdateUserLocation(IWebSocketServer *pWebSocketServer, int userid, QString lastip);
		static void UpdateMaxScoreGame(IWebSocketServer *pWebSocketServer, int gameid);
        static void UpdateUserRating(IWebSocketServer *pWebSocketServer, int nUserID);
        static void UpdateQuestSolved(IWebSocketServer *pWebSocketServer, int nQuestID);
        static void MailSend(IWebSocketServer *pWebSocketServer,  QString to, QString subject, QString content);
};

#endif // RUNTASKS_H
