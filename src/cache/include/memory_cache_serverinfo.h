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
        void serverStarted();
        int countQuests();
        int countQuestsAttempt();
        int countQuestsCompleted();
        void incrementQuests();
        void decrementQuests();
        void incrementQuestsAttempt();
        void incrementQuestsCompleted();
        void initCounters();

        QDateTime getServerStart();
		QJsonObject toJsonObject();

	private:
		IWebSocketServer *m_pWebSocketServer;
		QMap<QString, int> m_requestsCounter;
		QMutex m_mtxIncrementRequests;

        QString TAG;
        QDateTime m_dtServerStarted;
        int m_nCountQuests;
        int m_nCountQuestsAttempt;
        int m_nCountQuestsCompleted;
		
};

#endif // MEMORY_CACHE_SERVERINFO_H
