#include <employ_leaks.h>
#include <employ_database.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

REGISTRY_EMPLOY(EmployLeaks)

// ---------------------------------------------------------------------

EmployLeaks::EmployLeaks()
    : EmployBase(EmployLeaks::name(), {EmployDatabase::name()}) {
    TAG = EmployLeaks::name();
    m_mapCacheLeaks.clear();
    m_vectCacheLeaks.clear();
}

// ---------------------------------------------------------------------

bool EmployLeaks::init(){
    // load list of leaks to cache
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM leaks");
    if (!query.exec()){
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
        pModelLeak->setGameId(record.value("gameid").toInt());
        // TODO game_uuid
        pModelLeak->setName(record.value("name").toString().toStdString());
        pModelLeak->setContent(record.value("content").toString().toStdString());
        pModelLeak->setScore(record.value("score").toInt());
        pModelLeak->setSold(record.value("sold").toInt());

        // TODO created & updated
        // leak["created"] = record.value("created").toString().toHtmlEscaped();
        // leak["updated"] = record.value("updated").toString().toHtmlEscaped();
        if(m_mapCacheLeaks.count(sUuid)){
            Log::err(TAG, "Inconsistent list leaks in database uuid: " + sUuid);
        }else{
            m_mapCacheLeaks.insert(std::pair<std::string, ModelLeak*>(sUuid,pModelLeak));
        }
    }
    return true;
}

// ---------------------------------------------------------------------

int EmployLeaks::addLeak(ModelLeak* pModelLeak, std::string &sError){
    std::string sUuid = pModelLeak->uuid();
    std::string sGameUuid = pModelLeak->gameUuid();

    if(m_mapCacheLeaks.count(sUuid)){
        // pError = new Error(403, "Leak already exists with this uuid");
        return Employ::ALREADY_EXISTS;
	}

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    // TODO refactor on employ games
    // check the game
    {
        QSqlQuery query(db);
        query.prepare("SELECT id,uuid FROM games WHERE uuid = :game_uuid");
        query.bindValue(":game_uuid", QString(sGameUuid.c_str()));
        if (!query.exec()){
            sError = query.lastError().text().toStdString();
            return Employ::DATABASE_ERROR;
        }
        if (!query.next()){
            return Employ::GAME_NOT_FOUND;
        }

        QSqlRecord record = query.record();
        pModelLeak->setGameId(record.value("id").toInt());
    }

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
        query.bindValue(":name", QString(pModelLeak->name().c_str()));
        query.bindValue(":content", QString(pModelLeak->content().c_str()));
        query.bindValue(":score", pModelLeak->score());
        query.bindValue(":sold", pModelLeak->sold());

        if (!query.exec()){
            sError = query.lastError().text().toStdString();
            return Employ::DATABASE_ERROR;
        }
        int rowid = query.lastInsertId().toInt();
        pModelLeak->setLocalId(rowid);
    }

    // TODO set id
    m_mapCacheLeaks.insert(std::pair<std::string, ModelLeak*>(sUuid,pModelLeak));
    return Employ::OK;
}

// ---------------------------------------------------------------------

const ModelLeak* EmployLeaks::findLeakByUuid(std::string sUuid){
    std::map<std::string, ModelLeak*>::iterator it = m_mapCacheLeaks.find(sUuid);
    if (it != m_mapCacheLeaks.end()){
		return it->second; // TODO nullpointer exception
	}
	return NULL;
}

// ---------------------------------------------------------------------

int EmployLeaks::removeLeak(std::string sUuid){
    std::map<std::string, ModelLeak*>::iterator it = m_mapCacheLeaks.find(sUuid);
    if (it != m_mapCacheLeaks.end()){
        ModelLeak* pLeak = it->second;
        m_mapCacheLeaks.erase(it);
        delete pLeak;
		return true;
	}
	// TODO remove leak from database
	return false;
}

// ---------------------------------------------------------------------

int EmployLeaks::updateLeak(ModelLeak* pModelLeak){
    std::string sUuid = pModelLeak->uuid();
    std::map<std::string, ModelLeak*>::iterator it = m_mapCacheLeaks.find(sUuid);
    if (it != m_mapCacheLeaks.end()){
		it->second = pModelLeak;
		return true;
	}


	// TODO save changes by leak to database
	return false;
}

// ---------------------------------------------------------------------

nlohmann::json EmployLeaks::toJson(){
	nlohmann::json jsonLeaks;
	// TODO fill json leaks
	return jsonLeaks;
}

// ---------------------------------------------------------------------
