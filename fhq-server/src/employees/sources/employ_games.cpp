#include <employ_games.h>
#include <employ_settings.h>
#include <employ_database.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <sys/stat.h>

REGISTRY_EMPLOY(EmployGames)

// ---------------------------------------------------------------------

EmployGames::EmployGames()
    : EmployBase(EmployGames::name(), {EmployDatabase::name(), EmploySettings::name()}) {
    TAG = EmployGames::name();
}

// ---------------------------------------------------------------------

bool EmployGames::init(){
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
