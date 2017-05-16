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
		QJsonArray toJsonArray();

	private:
		IWebSocketServer *m_pWebSocketServer;
};

#endif // MEMORY_CACHE_SCOREBOARD_H
