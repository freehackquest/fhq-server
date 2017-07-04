#include "../headers/cmd_games_handler.h"
#include <QJsonArray>

CmdGamesHandler::CmdGamesHandler(){
	// m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
	// m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
}

QString CmdGamesHandler::cmd(){
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

const QVector<CmdInputDef> &CmdGamesHandler::inputs(){
	return m_vInputs;
};

QString CmdGamesHandler::description(){
	return "Method returned list of games";
}

QStringList CmdGamesHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGamesHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){

	QJsonArray games;
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM games ORDER BY games.date_start");
	query.exec();
	while (query.next()) {
		QSqlRecord record = query.record();
		QJsonObject game;
		game["id"] = record.value("id").toInt();
		game["uuid"] = record.value("uuid").toString();
		game["title"] = record.value("title").toString();
		game["type_game"] = record.value("type_game").toString();
		game["date_start"] = record.value("date_start").toString();
		game["date_stop"] = record.value("date_stop").toString();
		game["date_restart"] = record.value("date_restart").toString();
		game["description"] = record.value("description").toString();
		game["state"] = record.value("state").toString();
		game["form"] = record.value("form").toString();
		game["logo"] = record.value("logo").toString();
		game["organizators"] = record.value("organizators").toString();
		game["maxscore"] = record.value("maxscore").toInt();
		games.push_back(game);
	}

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = games;
		
	pWebSocketServer->sendMessage(pClient, jsonData);
}
