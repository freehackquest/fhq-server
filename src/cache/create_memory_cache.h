#ifndef CREATE_MEMORY_CACHE_H
#define CREATE_MEMORY_CACHE_H

#include <iserver.h>
#include <QVector>
#include <QDebug>

void create_memory_cache(QMap<QString, IMemoryCache *> &mapMemoryCache, IWebSocketServer *pWebSocketServer);
	
#endif // CREATE_MEMORY_CACHE_H
