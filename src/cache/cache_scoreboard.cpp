#include "headers/cache_scoreboard.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

// ICache
QString CacheScoreboard::name(){
	return "cache_scoreboard";
}

CacheScoreboard::CacheScoreboard(){
	
}

void CacheScoreboard::loadSync(){
	
}

QJsonArray CacheScoreboard::toJsonArray(){
	QJsonArray arr;
	return arr;
}
