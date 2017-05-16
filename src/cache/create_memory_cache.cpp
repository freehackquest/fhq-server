#include "create_memory_cache.h"
#include "headers/memory_cache_scoreboard.h"

void create_memory_cache(QMap<QString, IMemoryCache *> &mapMemoryCache, IWebSocketServer *pWebSocketServer){
	
	QVector<IMemoryCache *> vMemoryCache;
	vMemoryCache.push_back(new MemoryCacheScoreboard(pWebSocketServer));
	
	for(int i = 0; i < vMemoryCache.size(); i++){
		QString name = vMemoryCache[i]->name();
		if(mapMemoryCache.contains(name)){
			qDebug() << "[WARNING] memory cache object '" << name << "' - already registered and will be skipped";	
		}else{
			mapMemoryCache[name] = vMemoryCache[i];
		}
	}
}
