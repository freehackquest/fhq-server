#ifndef MEMORY_CACHE_SERVERINFO_H
#define MEMORY_CACHE_SERVERINFO_H

#include <iserver.h>

class MemoryCacheServerInfo : public IMemoryCache {
	public:
		// ICache
		virtual QString name();
		
		// class
		MemoryCacheServerInfo(IWebSocketServer *pWebSocketServer);
		
		void incrementRequests(QString cmd);
		QJsonObject toJsonObject();

	private:
		IWebSocketServer *m_pWebSocketServer;
		QMap<QString, int> m_requestsCounter;
		QMutex m_mtxIncrementRequests;
		
};

#endif // MEMORY_CACHE_SERVERINFO_H
