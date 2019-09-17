#include "employ_chats.h"
#include <employ_database.h>
#include <employ_games.h>
#include <employ_notify.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <QString>
#include <QVariant>

REGISTRY_WJSCPP_EMPLOY(EmployChats)

// ---------------------------------------------------------------------

EmployChats::EmployChats()
    : EmployBase(EmployChats::name(), {EmployDatabase::name(), EmployNotify::name()}) {
    TAG = EmployChats::name();
}

// ---------------------------------------------------------------------

bool EmployChats::init() {
    // load list of leaks to cache
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    EmployGames *pEmployGames = findEmploy<EmployGames>();

    /* QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM leaks");
    if (!query.exec()) {
        Log::err(TAG, query.lastError().text().toStdString());
        return false;
    }
    while (query.next()) {
        QSqlRecord record = query.record();
        ModelLeak* pModelLeak = new ModelLeak();
        QJsonObject leak;
        pModelLeak->setLocalId(record.value("id").toInt());
        std::string sUuid = record.value("uuid").toString().toStdString();
        pModelLeak->setUuid(sUuid);
        int nGameId = record.value("gameid").toInt();
        pModelLeak->setGameId(nGameId);
        ModelGame modelGame;
        if (pEmployGames->findGame(nGameId, modelGame)) {
            pModelLeak->setGameUuid(modelGame.uuid());
        } else {
            Log::err(TAG, "Game not found by localId: " + std::to_string(nGameId));
        }
        pModelLeak->setName(record.value("name").toString().toStdString());
        pModelLeak->setContent(record.value("content").toString().toStdString());
        pModelLeak->setScore(record.value("score").toInt());
        pModelLeak->setSold(record.value("sold").toInt());

        // TODO created & updated
        // leak["created"] = record.value("created").toString().toHtmlEscaped();
        // leak["updated"] = record.value("updated").toString().toHtmlEscaped();
        if (m_mapCacheLeaks.count(sUuid)) {
            Log::err(TAG, "Inconsistent list leaks in database uuid: " + sUuid);
        } else {
            m_mapCacheLeaks.insert(std::pair<std::string, ModelLeak*>(sUuid,pModelLeak));
        }
    }*/
    return true;
}

// ---------------------------------------------------------------------
