#include <memory_cache_serverinfo.h>

#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <log.h>
#include <employ_database.h>

// IMemoryCache
QString MemoryCacheServerInfo::name(){
	return "serverinfo";
}

// ---------------------------------------------------------------------

MemoryCacheServerInfo::MemoryCacheServerInfo(IWebSocketServer *pWebSocketServer){
    TAG = "MemoryCacheServerInfo";
	m_pWebSocketServer = pWebSocketServer;
    m_nCountQuests = 0;
    m_nCountQuestsAttempt = 0;
    m_nCountQuestsCompleted = 0;
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

nlohmann::json MemoryCacheServerInfo::toJson(){
    nlohmann::json jsonRes;
	foreach( QString key, m_requestsCounter.keys()){
		int count = m_requestsCounter.value(key);
        jsonRes[key.toStdString()] = count;
    }
    return jsonRes;
}

// ---------------------------------------------------------------------

void MemoryCacheServerInfo::serverStarted(){
    m_dtServerStarted = QDateTime::currentDateTimeUtc();
}

// ---------------------------------------------------------------------

QDateTime MemoryCacheServerInfo::getServerStart(){
    return m_dtServerStarted;
}

// ---------------------------------------------------------------------

int MemoryCacheServerInfo::countQuests(){
    return m_nCountQuests;
}

// ---------------------------------------------------------------------

int MemoryCacheServerInfo::countQuestsAttempt(){
    return m_nCountQuestsAttempt;
}

// ---------------------------------------------------------------------

int MemoryCacheServerInfo::countQuestsCompleted(){
    return m_nCountQuestsCompleted;
}

// ---------------------------------------------------------------------

void MemoryCacheServerInfo::incrementQuests(){
    m_nCountQuests++;
}

// ---------------------------------------------------------------------

void MemoryCacheServerInfo::decrementQuests(){
    m_nCountQuests--;
}

// ---------------------------------------------------------------------

void MemoryCacheServerInfo::incrementQuestsAttempt(){
    m_nCountQuestsAttempt++;
}

// ---------------------------------------------------------------------

void MemoryCacheServerInfo::incrementQuestsCompleted(){
    m_nCountQuestsCompleted++;
}

// ---------------------------------------------------------------------

void MemoryCacheServerInfo::initCounters(){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    // count quests
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM quest");
        if (!query.exec()){
            Log::err(TAG, query.lastError().text());
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            m_nCountQuests = record.value("cnt").toInt();
        }else{
            // TODO error
            m_nCountQuests = 0;
        }
    }

    // quest attempts
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM users_quests_answers");
        if (!query.exec()){
            Log::err(TAG, query.lastError().text());
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            m_nCountQuestsAttempt = record.value("cnt").toInt();
        }else{
            // TODO error
            m_nCountQuestsAttempt = 0;
        }
    }

    // completed
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM users_quests");
        if (!query.exec()){
            Log::err(TAG, query.lastError().text());
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            m_nCountQuestsCompleted = record.value("cnt").toInt();
        }else{
            // TODO error
            m_nCountQuestsCompleted = 0;
        }
    }
}

