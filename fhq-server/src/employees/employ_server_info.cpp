#include <employ_server_info.h>
#include <employees.h>
#include <employ_database.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <QString>
#include <QVariant>

REGISTRY_WJSCPP_EMPLOY(EmployServerInfo)

// ---------------------------------------------------------------------

EmployServerInfo::EmployServerInfo()
    : WsjcppEmployBase(EmployServerInfo::name(), {EmployDatabase::name()}) {
    m_nCountQuests = 0;
    m_nCountQuestsAttempt = 0;
    m_nCountQuestsCompleted = 0;
}

// ---------------------------------------------------------------------

bool EmployServerInfo::init() {

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    // count quests
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM quest");
        if (!query.exec()) {
            WsjcppLog::err(TAG, query.lastError().text().toStdString());
            return false;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            m_nCountQuests = record.value("cnt").toInt();
        } else {
            // TODO error
            m_nCountQuests = 0;
        }
    }

    // quest attempts
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM users_quests_answers");
        if (!query.exec()) {
            WsjcppLog::err(TAG, query.lastError().text().toStdString());
            return false;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            m_nCountQuestsAttempt = record.value("cnt").toInt();
        } else {
            // TODO error
            m_nCountQuestsAttempt = 0;
        }
    }

    // completed
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM users_quests");
        if (!query.exec()) {
            WsjcppLog::err(TAG, query.lastError().text().toStdString());
            return false;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            m_nCountQuestsCompleted = record.value("cnt").toInt();
        } else {
            // TODO error
            m_nCountQuestsCompleted = 0;
        }
    }

    return true;
}

// ---------------------------------------------------------------------

bool EmployServerInfo::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------

void EmployServerInfo::incrementRequests(const std::string& cmd) {
    QMutexLocker locker (&m_mtxIncrementRequests);
    if (m_requestsCounter.contains(cmd)) {
        m_requestsCounter[cmd] = m_requestsCounter[cmd]+1;
    } else {
        m_requestsCounter[cmd] = 1;
    }
}

// ---------------------------------------------------------------------

nlohmann::json EmployServerInfo::toJson() {
    nlohmann::json jsonRes;
    foreach( std::string key, m_requestsCounter.keys()) {
        int count = m_requestsCounter.value(key);
        jsonRes[key] = count;
    }
    return jsonRes;
}

// ---------------------------------------------------------------------

void EmployServerInfo::serverStarted() {
    // TODO redesign this to helpers lib
    m_dtServerStarted = QDateTime::currentDateTimeUtc();
}

// ---------------------------------------------------------------------

QDateTime EmployServerInfo::getServerStart() {
    return m_dtServerStarted;
}

// ---------------------------------------------------------------------

int EmployServerInfo::countQuests() {
    return m_nCountQuests;
}

// ---------------------------------------------------------------------

int EmployServerInfo::countQuestsAttempt() {
    return m_nCountQuestsAttempt;
}

// ---------------------------------------------------------------------

int EmployServerInfo::countQuestsCompleted() {
    return m_nCountQuestsCompleted;
}

// ---------------------------------------------------------------------

void EmployServerInfo::incrementQuests() {
    m_nCountQuests++;
}

// ---------------------------------------------------------------------

void EmployServerInfo::decrementQuests() {
    m_nCountQuests--;
}

// ---------------------------------------------------------------------

void EmployServerInfo::incrementQuestsAttempt() {
    m_nCountQuestsAttempt++;
}

// ---------------------------------------------------------------------

void EmployServerInfo::incrementQuestsCompleted() {
    m_nCountQuestsCompleted++;
}

// ---------------------------------------------------------------------

nlohmann::json EmployServerInfo::developers() {
    nlohmann::json jsonDevelopers = nlohmann::json::array();

    nlohmann::json jsonDev1;
    jsonDev1["name"] = "Evgenii Sopov";
    jsonDev1["email"] = "mrseakg@gmail.com";
    jsonDevelopers.push_back(jsonDev1);

    nlohmann::json jsonDev2;
    jsonDev2["name"] = "Igor Polyakov";
    jsonDev2["email"] = "?";
    jsonDevelopers.push_back(jsonDev2);

    nlohmann::json jsonDev3;
    jsonDev3["name"] = "Sergey Ushev";
    jsonDev3["email"] = "sergo.moreno@gmail.com";
    jsonDevelopers.push_back(jsonDev3);

    nlohmann::json jsonDev4;
    jsonDev4["name"] = "Danil Dudkin";
    jsonDev4["email"] = "shikamaru740@gmail.com";
    jsonDevelopers.push_back(jsonDev4);

    return jsonDevelopers;
}

// ---------------------------------------------------------------------
