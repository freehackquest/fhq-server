#include <employ_server_config.h>
#include <employ_server_info.h>
#include <employ_database.h>
#include <QSqlQuery>
#include <QSqlRecord>


REGISTRY_EMPLOY(EmployServerInfo)

// ---------------------------------------------------------------------

EmployServerInfo::EmployServerInfo()
    : EmployBase(EmployServerInfo::name(), {EmployDatabase::name()}) {
    m_nCountQuests = 0;
    m_nCountQuestsAttempt = 0;
    m_nCountQuestsCompleted = 0;
}

// ---------------------------------------------------------------------

bool EmployServerInfo::init(){

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    // count quests
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM quest");
        if (!query.exec()){
            Log::err(TAG, query.lastError().text());
            return false;
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
            return false;
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
            return false;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            m_nCountQuestsCompleted = record.value("cnt").toInt();
        }else{
            // TODO error
            m_nCountQuestsCompleted = 0;
        }
    }

    return true;
}

// ---------------------------------------------------------------------

void EmployServerInfo::incrementRequests(const std::string& cmd){
    QMutexLocker locker (&m_mtxIncrementRequests);
    if(m_requestsCounter.contains(cmd)){
        m_requestsCounter[cmd] = m_requestsCounter[cmd]+1;
    }else{
        m_requestsCounter[cmd] = 1;
    }
}

// ---------------------------------------------------------------------

nlohmann::json EmployServerInfo::toJson(){
    nlohmann::json jsonRes;
    foreach( std::string key, m_requestsCounter.keys()){
        int count = m_requestsCounter.value(key);
        jsonRes[key] = count;
    }
    return jsonRes;
}

// ---------------------------------------------------------------------

void EmployServerInfo::serverStarted(){
    m_dtServerStarted = QDateTime::currentDateTimeUtc();
}

// ---------------------------------------------------------------------

QDateTime EmployServerInfo::getServerStart(){
    return m_dtServerStarted;
}

// ---------------------------------------------------------------------

int EmployServerInfo::countQuests(){
    return m_nCountQuests;
}

// ---------------------------------------------------------------------

int EmployServerInfo::countQuestsAttempt(){
    return m_nCountQuestsAttempt;
}

// ---------------------------------------------------------------------

int EmployServerInfo::countQuestsCompleted(){
    return m_nCountQuestsCompleted;
}

// ---------------------------------------------------------------------

void EmployServerInfo::incrementQuests(){
    m_nCountQuests++;
}

// ---------------------------------------------------------------------

void EmployServerInfo::decrementQuests(){
    m_nCountQuests--;
}

// ---------------------------------------------------------------------

void EmployServerInfo::incrementQuestsAttempt(){
    m_nCountQuestsAttempt++;
}

// ---------------------------------------------------------------------

void EmployServerInfo::incrementQuestsCompleted(){
    m_nCountQuestsCompleted++;
}

// ---------------------------------------------------------------------
