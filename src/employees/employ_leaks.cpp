#include <employ_leaks.h>
#include <employ_database.h>
#include <employ_games.h>
#include <employ_notify.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <QString>
#include <QVariant>

REGISTRY_WJSCPP_EMPLOY(EmployLeaks)

// ---------------------------------------------------------------------

EmployLeaks::EmployLeaks()
    : WsjcppEmployBase(EmployLeaks::name(), {EmployDatabase::name(), EmployNotify::name(), EmployGames::name()}) {
    TAG = EmployLeaks::name();
    m_mapCacheLeaks.clear();
    m_vectCacheLeaks.clear();
}

// ---------------------------------------------------------------------

bool EmployLeaks::init() {
    // load list of leaks to cache
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM leaks");
    if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
        return false;
    }
    while (query.next()) {
        QSqlRecord record = query.record();
        ModelLeak* pModelLeak = new ModelLeak();
        pModelLeak->setLocalId(record.value("id").toInt());
        std::string sUuid = record.value("uuid").toString().toStdString();
        pModelLeak->setUuid(sUuid);
        int nGameId = record.value("gameid").toInt();
        pModelLeak->setGameId(nGameId);
        ModelGame modelGame;
        if (pEmployGames->findGame(nGameId, modelGame)) {
            pModelLeak->setGameUuid(modelGame.uuid());
        } else {
            WsjcppLog::err(TAG, "Game not found by localId: " + std::to_string(nGameId));
        }
        pModelLeak->setName(record.value("name").toString().toStdString());
        pModelLeak->setContent(record.value("content").toString().toStdString());
        pModelLeak->setScore(record.value("score").toInt());
        pModelLeak->setSold(record.value("sold").toInt());

        // TODO created & updated
        // leak["created"] = record.value("created").toString().toHtmlEscaped();
        // leak["updated"] = record.value("updated").toString().toHtmlEscaped();
        if (m_mapCacheLeaks.count(sUuid)) {
            WsjcppLog::err(TAG, "Inconsistent list leaks in database uuid: " + sUuid);
        } else {
            m_mapCacheLeaks.insert(std::pair<std::string, ModelLeak*>(sUuid,pModelLeak));
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployLeaks::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------

int EmployLeaks::addLeak(ModelLeak* pModelLeak, std::string &sError) {
    std::string sUuid = pModelLeak->uuid();
    std::string sGameUuid = pModelLeak->gameUuid();
    std::string sName = pModelLeak->name();

    if (m_mapCacheLeaks.count(sUuid)) {
        // pError = new Error(403, "Leak already exists with this uuid");
        return EmployResult::ALREADY_EXISTS;
    }

    // check the game
    EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();
    ModelGame modelGame;
    if (!pEmployGames->findGame(sGameUuid, modelGame)) {
        return EmployResult::GAME_NOT_FOUND;
    }

    pModelLeak->setGameId(modelGame.localId());

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO leaks( "
            " uuid, gameid, name, "
            " content, score, sold, "
            " created, updated "
            ") "
            "VALUES( "
            " :uuid, :gameid, :name,"
            " :content, :score, :sold, "
            " NOW(), NOW()"
            ");");
        query.bindValue(":uuid", QString(sUuid.c_str()));
        query.bindValue(":gameid", pModelLeak->gameId());
        query.bindValue(":name", QString(sName.c_str()));
        query.bindValue(":content", QString(pModelLeak->content().c_str()));
        query.bindValue(":score", pModelLeak->score());
        query.bindValue(":sold", pModelLeak->sold());

        if (!query.exec()) {
            sError = query.lastError().text().toStdString();
            return EmployResult::DATABASE_ERROR;
        }
        int rowid = query.lastInsertId().toInt();
        pModelLeak->setLocalId(rowid);
    }

    m_mapCacheLeaks.insert(std::pair<std::string, ModelLeak*>(sUuid,pModelLeak));

    EmployNotify *pEmployNotify = findWsjcppEmploy<EmployNotify>();
    pEmployNotify->notifyInfo("leaks", "New [leak#" + sUuid + "] " + sName);

    return EmployResult::OK;
}

// ---------------------------------------------------------------------

const ModelLeak* EmployLeaks::findLeakByUuid(std::string sUuid) {
    std::map<std::string, ModelLeak*>::iterator it = m_mapCacheLeaks.find(sUuid);
    if (it != m_mapCacheLeaks.end()) {
        return it->second; // TODO nullpointer exception
    }
    return NULL;
}

// ---------------------------------------------------------------------

int EmployLeaks::removeLeak(std::string sUuid) {
    std::map<std::string, ModelLeak*>::iterator it = m_mapCacheLeaks.find(sUuid);
    if (it != m_mapCacheLeaks.end()) {
        ModelLeak* pLeak = it->second;
        m_mapCacheLeaks.erase(it);
        delete pLeak;
        return true;
    }
    // TODO remove leak from database
    return false;
}

// ---------------------------------------------------------------------

int EmployLeaks::updateLeak(ModelLeak* pModelLeak) {
    std::string sUuid = pModelLeak->uuid();
    std::map<std::string, ModelLeak*>::iterator it = m_mapCacheLeaks.find(sUuid);
    if (it != m_mapCacheLeaks.end()) {
        it->second = pModelLeak;
        return true;
    }

    // TODO save changes by leak to database
    return false;
}

// ---------------------------------------------------------------------

nlohmann::json EmployLeaks::toJson() {
    nlohmann::json jsonLeaks;
    // TODO fill json leaks
    return jsonLeaks;
}

// ---------------------------------------------------------------------
