#ifndef MEMORY_CACHE_SCOREBOARD_H
#define MEMORY_CACHE_SCOREBOARD_H

#include <iserver.h>

class MemoryCacheScoreboard : public IMemoryCache {
	public:
		// ICache
		virtual QString name();
		
		// class
		MemoryCacheScoreboard(IWebSocketServer *pWebSocketServer);
		
		void loadSync();
		int count();
		QJsonArray toJsonArray();

	private:
		IWebSocketServer *m_pWebSocketServer;
		
		struct User{
			int userid = 0;
			QString logo = "";
			QString nick = "";
            QString university = "";
		};
		
		struct ScoreboardRow {
			int place = 0;
			int rating = 0;
			QVector<User *> vUsers;
		};
		QVector<ScoreboardRow *> m_vRows;
		int findScoreboardRowByRating(int rating);
};

#endif // MEMORY_CACHE_SCOREBOARD_H
