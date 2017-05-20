#include "../headers/memory_cache_serverinfo.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

// IMemoryCache
QString MemoryCacheServerInfo::name(){
	return "serverinfo";
}

// ---------------------------------------------------------------------

MemoryCacheServerInfo::MemoryCacheServerInfo(IWebSocketServer *pWebSocketServer){
	m_pWebSocketServer = pWebSocketServer;
}

// ---------------------------------------------------------------------

void MemoryCacheServerInfo::incrementRequests(QString cmd){
	QMutexLocker locker (&m_mtxIncrementRequests);
	if(m_requestsCounter.contains(cmd)){
		m_requestsCounter[cmd] = m_requestsCounter[cmd]+1;
	}else{
		m_requestsCounter[cmd] = 1;
	}
}

// ---------------------------------------------------------------------

QJsonObject MemoryCacheServerInfo::toJsonObject(){
	QJsonObject res;
	foreach( QString key, m_requestsCounter.keys()){
		int count = m_requestsCounter.value(key);
		res[key] = count;
	}
	return res;
}

// ---------------------------------------------------------------------
