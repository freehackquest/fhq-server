#include "../headers/memory_cache_scoreboard.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

// IMemoryCache
QString MemoryCacheScoreboard::name(){
	return "scoreboard";
}

// class
MemoryCacheScoreboard::MemoryCacheScoreboard(IWebSocketServer *pWebSocketServer){
	m_pWebSocketServer = pWebSocketServer;
}

void MemoryCacheScoreboard::loadSync(){
	QSqlDatabase db = *(m_pWebSocketServer->database());
}

QJsonArray MemoryCacheScoreboard::toJsonArray(){
	QJsonArray arr;
	return arr;
}
