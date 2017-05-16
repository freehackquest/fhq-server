#ifndef CACHE_SCOREBOARD_H
#define CACHE_SCOREBOARD_H

#include <iserver.h>

class CacheScoreboard : ICache{
	public:
		// ICache
		virtual QString name();
		
		// other
		CacheScoreboard();
		
		void loadSync();
		QJsonArray toJsonArray();

	private:
};

#endif // CACHE_SCOREBOARD_H
