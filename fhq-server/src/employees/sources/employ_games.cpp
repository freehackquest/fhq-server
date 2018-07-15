#include <employ_games.h>
#include <employ_database.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

REGISTRY_EMPLOY(EmployGames)

// ---------------------------------------------------------------------

EmployGames::EmployGames()
    : EmployBase(EmployGames::name(), {EmployDatabase::name()}) {
    TAG = EmployGames::name();
}

// ---------------------------------------------------------------------

bool EmployGames::init(){
    // load list of games to cache
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM games");
    if (!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
        return false;
    }
    while (query.next()) {
        QSqlRecord record = query.record();
        ModelGame* pModelGame = new ModelGame();
        pModelGame->setLocalId(record.value("id").toInt());
        std::string sUuid = record.value("uuid").toString().toStdString();
        pModelGame->setUuid(sUuid);
        pModelGame->setName(record.value("name").toString().toStdString());

        if(m_mapCacheGames.count(sUuid)){
            Log::err(TAG, "Inconsistent list games in database uuid: " + sUuid);
            return false;
        }else{
            m_mapCacheGames.insert(std::pair<std::string, ModelGame*>(sUuid,pModelGame));
            m_vectCacheGame.push_back(pModelGame);
        }
    }
    return true;
}

// ---------------------------------------------------------------------
