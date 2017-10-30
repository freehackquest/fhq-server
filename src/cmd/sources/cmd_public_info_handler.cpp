#include <cmd_public_info_handler.h>

CmdPublicInfoHandler::CmdPublicInfoHandler(){
	
}

QString CmdPublicInfoHandler::cmd(){
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

const QVector<CmdInputDef> &CmdPublicInfoHandler::inputs(){
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
        query.prepare("SELECT COUNT(*) cnt FROM quest");
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonQuests["count"] = record.value("cnt").toInt();
        }else{
            // TODO error
            jsonQuests["count"] = 0;
        }
    }


    // users_quests_answers
    // TODO get from cache
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM users_quests_answers");
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonQuests["attempts"] = record.value("cnt").toInt();
        }else{
            // TODO error
            jsonQuests["attempts"] = 0;
        }
    }

    // TODO get from cache
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM users_quests");
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonQuests["solved"] = record.value("cnt").toInt();
        }else{
            // TODO error
            jsonQuests["solved"] = 0;
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

	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["m"] = QJsonValue(m);
    jsonData["quests"] = jsonQuests;
    jsonData["winners"] = jsonWinners;
    jsonData["cities"] = jsonCities;
	jsonData["connectedusers"] = pWebSocketServer->getConnectedUsers();
	pWebSocketServer->sendMessage(pClient, jsonData);
}
