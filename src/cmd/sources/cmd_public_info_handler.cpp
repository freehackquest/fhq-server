#include <cmd_public_info_handler.h>
#include <memory_cache_serverinfo.h>

CmdPublicInfoHandler::CmdPublicInfoHandler(){
	
}

std::string CmdPublicInfoHandler::cmd(){
    return "public_info";
}

bool CmdPublicInfoHandler::accessUnauthorized(){
	return true;
}

bool CmdPublicInfoHandler::accessUser(){
	return true;
}

bool CmdPublicInfoHandler::accessTester(){
	return true;
}

bool CmdPublicInfoHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdPublicInfoHandler::inputs(){
	return m_vInputs;
};

QString CmdPublicInfoHandler::description(){
    return "Method return public information about server";
}

QStringList CmdPublicInfoHandler::errors(){
	QStringList	list;
	return list;
}

void CmdPublicInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject /*obj*/){
	QJsonObject jsonData;
    QJsonObject jsonQuests;
    QJsonArray jsonWinners;
    QJsonArray jsonCities;

    // fill quests info
    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serverinfo");
    if(pMemoryCache == NULL){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
        return;
    }
    MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);
    jsonQuests["count"] = pMemoryCacheServerInfo->countQuests();
    jsonQuests["attempts"] = pMemoryCacheServerInfo->countQuestsAttempt();
    jsonQuests["solved"] = pMemoryCacheServerInfo->countQuestsCompleted();

    QSqlDatabase db = *(pWebSocketServer->database());

    int nMoreThen = 2;
    int nCitiesLimit = 20;

    // TODO load from settings
    // nMoreThen
    // nCitiesLimit

    // cities
    // TODO get from cache
    {
        QSqlQuery query(db);
        query.prepare(""
                      "SELECT * FROM ("
                      "     SELECT city, COUNT(*) cnt FROM users WHERE city IS NOT NULL AND city <> '' GROUP BY city ORDER BY cnt DESC "
                      ") AS cities "
                      "WHERE cities.cnt > :morethan "
                      "LIMIT 0,:citieslimit");
        query.bindValue(":morethan", nMoreThen);
        query.bindValue(":citieslimit", nCitiesLimit);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        while(query.next()) {
            QSqlRecord record = query.record();
            QJsonObject city;
            city["cnt"] = record.value("cnt").toInt();
            city["city"] = record.value("city").toString().toHtmlEscaped();
            jsonCities.append(city);
        }
    }



    // TODO get from cache
    {
        QSqlQuery query(db);
        query.prepare("SELECT u.nick, u.university, u.rating FROM users u WHERE u.role = :role ORDER BY u.rating DESC LIMIT 0,10");
        query.bindValue(":role", "user");
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        int nPlace = 1;
        while(query.next()) {
            QSqlRecord record = query.record();
            QJsonObject user;
            user["place"] = nPlace;
            user["nick"] = record.value("nick").toString().toHtmlEscaped();
            user["university"] = record.value("university").toString().toHtmlEscaped();
            user["rating"] = record.value("rating").toInt();
            jsonWinners.append(user);
            nPlace++;
        }
    }

    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["m"] = QJsonValue(m);
    jsonData["quests"] = jsonQuests;
    jsonData["winners"] = jsonWinners;
    jsonData["cities"] = jsonCities;
	jsonData["connectedusers"] = pWebSocketServer->getConnectedUsers();
	pWebSocketServer->sendMessage(pClient, jsonData);
}
