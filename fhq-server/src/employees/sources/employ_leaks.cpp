#include <employ_leaks.h>
#include <employ_database.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

REGISTRY_EMPLOY(EmployLeaks)


int EmployLeaks::OK = 0;
int EmployLeaks::GAME_NOT_FOUND = 1;
int EmployLeaks::ALREADY_EXISTS = 2;
int EmployLeaks::DATABASE_ERROR = 3;

// ---------------------------------------------------------------------

EmployLeaks::EmployLeaks()
    : EmployBase(EmployLeaks::name(), {EmployDatabase::name()}) {
    m_mapCacheLeaks.clear();
    m_vectCacheLeaks.clear();
}

// ---------------------------------------------------------------------

bool EmployLeaks::init(){
	
	// TODO load leaks from files or database
    return true;
}

// ---------------------------------------------------------------------

int EmployLeaks::addLeak(ModelLeak* pModelLeak, std::string &sError){
    std::string sUuid = pModelLeak->uuid();
    std::string sGameUuid = pModelLeak->gameUuid();

    if(m_mapCacheLeaks.count(sUuid)){
        // pError = new Error(403, "Leak already exists with this uuid");
        return EmployLeaks::ALREADY_EXISTS;
	}

    m_mapCacheLeaks.insert(std::pair<std::string, ModelLeak*>(sUuid,pModelLeak));

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    int nGameId = 0;
    // check the game
    {
        QSqlQuery query(db);
        query.prepare("SELECT id,uuid FROM games WHERE uuid = :game_uuid");
        query.bindValue(":game_uuid", QString(sGameUuid.c_str()));
        if (!query.exec()){
            sError = query.lastError().text().toStdString();
            return EmployLeaks::DATABASE_ERROR;
        }
        if (!query.next()){
            return EmployLeaks::GAME_NOT_FOUND;
        }

        QSqlRecord record = query.record();
        nGameId = record.value("id").toInt();
        pModelLeak->setGameId(nGameId);
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
            return EmployLeaks::DATABASE_ERROR;
        }
    }

    return EmployLeaks::OK;
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
