#include <employ_games.h>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_notify.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <sys/stat.h>


REGISTRY_EMPLOY(EmployGames)

// ---------------------------------------------------------------------

EmployGames::EmployGames()
    : EmployBase(EmployGames::name(), {EmployDatabase::name(), EmploySettings::name(), EmployNotify::name()}) {
    TAG = EmployGames::name();
}

// ---------------------------------------------------------------------

bool EmployGames::init(){
    // TODO mutex
    // check the access to games folder
    EmploySettings *pSettings = findEmploy<EmploySettings>();
    QString sBasePath = pSettings->getSettString("server_folder_public");
    std::string targetTestFile = sBasePath.toStdString();
    targetTestFile += "games/test";

    FILE * pFile = fopen (targetTestFile.c_str(), "wb");
    if(pFile == NULL){
        Log::err(TAG, "Cannot access to write " + targetTestFile);
        return false;
    }
    fclose(pFile);

    if( remove( targetTestFile.c_str() ) != 0 ){
        Log::err(TAG, "Could not delete file " + targetTestFile);
        return false;
    }

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

ModelGame* EmployGames::findGameByLocalId(int nLocalId){
    // TODO mutex
    for(int i = 0; i < m_vectCacheGame.size(); i++){
        if(m_vectCacheGame[i]->localId() == nLocalId){
            return m_vectCacheGame[i];
        }
    }
    return NULL;
}

// ---------------------------------------------------------------------

ModelGame* EmployGames::findGameByUuid(const std::string &sUuid){
    if(!m_mapCacheGames.count(sUuid)){
        return NULL;
    }
    return m_mapCacheGames.at(sUuid);
}

// ---------------------------------------------------------------------

EmployResult EmployGames::addGame(ModelGame* pModelGame, std::string &sError){
    // TODO mutex
    std::string sUuid = pModelGame->uuid();

    if(m_mapCacheGames.count(sUuid)){
        return EmployResult::ALREADY_EXISTS;
    }

    if(pModelGame->name().length() == 0){
        return EmployResult::ERROR_NAME_IS_EMPTY;
    }

    std::string sName = pModelGame->name();

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO games("
            "		uuid, title, type_game,"
            "		date_start, date_stop, date_restart,"
            "		description,"
            "		organizators,"
            "		state, form, rules,"
            "		maxscore,"
            "       date_create, " // TODO change created
            "		date_change "  // TODO change updated
            "	)"
            "	VALUES("
            "		:uuid, :name, :type_game,"
            "		:date_start, :date_stop, :date_restart,"
            "		:description,"
            "		:organizators,"
            "		:state, :form, :rules,"
            "		:maxscore,"
            "		NOW(),"
            "		NOW()"
            "	)");
        query.bindValue(":uuid", QString(sUuid.c_str()));
        query.bindValue(":name", QString(sName.c_str()));
        query.bindValue(":type_game", QString(pModelGame->type().c_str()));
        query.bindValue(":date_start", QString(pModelGame->dateStart().c_str()));
        query.bindValue(":date_stop", QString(pModelGame->dateStop().c_str()));
        query.bindValue(":date_restart", QString(pModelGame->dateRestart().c_str()));
        query.bindValue(":description", QString(pModelGame->description().c_str()));
        query.bindValue(":organizators", QString(pModelGame->organizators().c_str()));
        query.bindValue(":state", QString(pModelGame->state().c_str()));
        query.bindValue(":form", QString(pModelGame->form().c_str()));
        query.bindValue(":rules", "");
        query.bindValue(":maxscore", 0);

        if (!query.exec()){
            sError = query.lastError().text().toStdString();
            return EmployResult::DATABASE_ERROR;
        }

        int rowid = query.lastInsertId().toInt();
        pModelGame->setLocalId(rowid);
    }

    m_mapCacheGames.insert(std::pair<std::string, ModelGame*>(sUuid,pModelGame));
    m_vectCacheGame.push_back(pModelGame);

    EmployNotify *pEmployNotify = findEmploy<EmployNotify>();
    pEmployNotify->notifyInfo("games", "New [game#" + sUuid + "] " + sName);
    return EmployResult::OK;
}

// ---------------------------------------------------------------------

EmployResult EmployGames::removeGame(const std::string &sUuid){
    // TODO mutex
    if(!m_mapCacheGames.count(sUuid)){
        return EmployResult::GAME_NOT_FOUND;
    }
    m_mapCacheGames.erase(sUuid);

    // TODO also remove from vect

    // TODO remove from database
    return EmployResult::OK;
}


// ---------------------------------------------------------------------
// TODO move to helpers

bool EmployGames::dir_exists(const char* filePath)
{
    //The variable that holds the file information
    struct stat fileAtt; //the type stat and function stat have exactly the same names, so to refer the type, we put struct before it to indicate it is an structure.

    //Use the stat function to get the information
    if (stat(filePath, &fileAtt) != 0){ //start will be 0 when it succeeds
        return false;
    }

    //S_ISREG is a macro to check if the filepath referers to a file. If you don't know what a macro is, it's ok, you can use S_ISREG as any other function, it 'returns' a bool.
    return S_ISREG(fileAtt.st_mode);
}

// ---------------------------------------------------------------------

