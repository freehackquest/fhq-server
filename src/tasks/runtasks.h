#ifndef RUNTASKS_H
#define RUNTASKS_H

#include <iserver.h>
#include <QString>

class RunTasks {
	public:
		static void AddPublicEvents(IWebSocketServer *pWebSocketServer, QString type, QString message);
		static void UpdateUserLocation(IWebSocketServer *pWebSocketServer, int userid, QString lastip);
		static void UpdateMaxScoreGame(IWebSocketServer *pWebSocketServer, int gameid);
};

#endif // RUNTASKS_H
