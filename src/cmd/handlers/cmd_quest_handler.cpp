#include "../headers/cmd_quest_handler.h"
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdQuestHandler::CmdQuestHandler(){
	m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
}

QString CmdQuestHandler::cmd(){
	return "quest";
}

bool CmdQuestHandler::accessUnauthorized(){
	return true;
}

bool CmdQuestHandler::accessUser(){
	return true;
}

bool CmdQuestHandler::accessTester(){
	return true;
}

bool CmdQuestHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdQuestHandler::inputs(){
	return m_vInputs;
};

QString CmdQuestHandler::description(){
	return "Update the quest info";
}

QStringList CmdQuestHandler::errors(){
	QStringList	list;
	return list;
}

void CmdQuestHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	QSqlDatabase db = *(pWebSocketServer->database());

	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	bool bAdmin = pUserToken != NULL && pUserToken->isAdmin();

	int nQuestID = obj["questid"].toInt();
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM quest WHERE idquest = :questid");
		query.bindValue(":questid", nQuestID);
		if(!query.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Error(500, query.lastError().text()));
			return;
		}
		
		if (query.next()) {
			QSqlRecord record = query.record();
			QJsonObject quest;
			quest["id"] = record.value("idquest").toInt();
			int nGameID = record.value("gameid").toInt();
			QString sState = record.value("state").toString();
			
			if(sState == "open" || bAdmin){
				quest["gameid"] = nGameID;
				quest["name"] = record.value("name").toString();
				quest["text"] = record.value("text").toString();
				quest["answer_format"] = record.value("answer_format").toString();
				quest["score"] = record.value("score").toInt();
				quest["subject"] = record.value("subject").toString();
				quest["copyright"] = record.value("copyright").toString();
				quest["state"] = record.value("state").toString();
				quest["author"] = record.value("author").toString();
				quest["count_user_solved"] = record.value("count_user_solved").toString();
			}
			
			if(bAdmin){
				quest["answer"] = record.value("answer").toString();
				quest["description_state"] = record.value("description_state").toString();
			}
			jsonData["quest"] = quest;
			
			QJsonObject game;
			{
				QSqlQuery query_game(db);
				query_game.prepare("SELECT * FROM games WHERE id = :id");
				query_game.bindValue(":id", nGameID);
				if(!query_game.exec()){
					pWebSocketServer->sendMessageError(pClient, cmd(), Error(500, query_game.lastError().text()));
					return;
				}
				if (query_game.next()) {
					QSqlRecord record_game = query_game.record();
					game["id"] = record_game.value("id").toInt();
					game["title"] = record_game.value("title").toString();
				}else{
					pWebSocketServer->sendMessageError(pClient, cmd(), Error(404, "Game not found"));
					return;
				}
			}
			quest["game"] = game;
	
		}else{
			pWebSocketServer->sendMessageError(pClient, cmd(), Error(404, "Quest not found"));
			return;
		}
	}
	
	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
