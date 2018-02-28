#include <cmd_games_handler.h>
#include <QJsonArray>
#include <memory_cache_serversettings.h>

CmdGamesHandler::CmdGamesHandler(){
	// m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
	// m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
}

std::string CmdGamesHandler::cmd(){
	return "games";
}

bool CmdGamesHandler::accessUnauthorized(){
	return true;
}

bool CmdGamesHandler::accessUser(){
	return true;
}

bool CmdGamesHandler::accessTester(){
	return true;
}

bool CmdGamesHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdGamesHandler::inputs(){
	return m_vInputs;
};

QString CmdGamesHandler::description(){
	return "Method returned list of games";
}

QStringList CmdGamesHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGamesHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject /*obj*/){

    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serversettings");
    if(pMemoryCache == NULL){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
        return;
    }

    MemoryCacheServerSettings *pMemoryCacheServerSettings = dynamic_cast<MemoryCacheServerSettings*>(pMemoryCache);
    QString base_url = pMemoryCacheServerSettings->getSettString("server_folder_games_url");

	QJsonArray games;
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM games ORDER BY games.date_start");

    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }

    while (query.next()) {
		QSqlRecord record = query.record();
		QJsonObject game;
        int nGameID = record.value("id").toInt();
        game["id"] = nGameID;
		game["uuid"] = record.value("uuid").toString();
		game["title"] = record.value("title").toString();
		game["type_game"] = record.value("type_game").toString();
		game["date_start"] = record.value("date_start").toString();
		game["date_stop"] = record.value("date_stop").toString();
		game["date_restart"] = record.value("date_restart").toString();
		game["description"] = record.value("description").toString();
		game["state"] = record.value("state").toString();
		game["form"] = record.value("form").toString();
        game["logo"] = base_url + QString::number(nGameID) + ".png";
		game["organizators"] = record.value("organizators").toString();
		game["maxscore"] = record.value("maxscore").toInt();
		games.push_back(game);
	}

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["data"] = games;
		
	pWebSocketServer->sendMessage(pClient, jsonData);
}
