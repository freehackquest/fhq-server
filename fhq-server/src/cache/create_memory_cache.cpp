#include "create_memory_cache.h"
#include <log.h>
#include <memory_cache_scoreboard.h>

void create_memory_cache(QMap<QString, IMemoryCache *> &mapMemoryCache, IWebSocketServer *pWebSocketServer){
	QString TAG = "create_memory_cache";
	QVector<IMemoryCache *> vMemoryCache;
	vMemoryCache.push_back(new MemoryCacheScoreboard(pWebSocketServer));
	
	for(int i = 0; i < vMemoryCache.size(); i++){
		QString name = vMemoryCache[i]->name();
		if(mapMemoryCache.contains(name)){
			Log::warn(TAG, "memory cache object '" + name + "' - already registered and will be skipped");
		}else{
			mapMemoryCache[name] = vMemoryCache[i];
		}
	}
}
