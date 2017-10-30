#include <cmd_game_delete_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameDeleteHandler::CmdGameDeleteHandler(){
    m_vInputs.push_back(CmdInputDef("gameid").integer_().required().description("Game ID"));
    m_vInputs.push_back(CmdInputDef("admin_password").string_().required().description("Admin Password"));
}

QString CmdGameDeleteHandler::cmd(){
    return "game_delete";
}

bool CmdGameDeleteHandler::accessUnauthorized(){
	return false;
}

bool CmdGameDeleteHandler::accessUser(){
	return false;
}

bool CmdGameDeleteHandler::accessTester(){
	return false;
}

bool CmdGameDeleteHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdGameDeleteHandler::inputs(){
	return m_vInputs;
};

QString CmdGameDeleteHandler::description(){
    return "Remove game and all quests";
}

QStringList CmdGameDeleteHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameDeleteHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

	int nGameID = obj["gameid"].toInt();
	QString sAdminPassword = obj["admin_password"].toString();
	
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
    int nUserID = pUserToken->userid();
    
	QSqlDatabase db = *(pWebSocketServer->database());
	
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM users WHERE id = :userid");
		query.bindValue(":userid", nUserID);
		if(!query.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
			return;
		}

		QString sPass = "";
		QString sEmail = "";

		if (query.next()) {
			QSqlRecord record = query.record();
			sEmail = record.value("email").toString();
			sPass = record.value("pass").toString();
		}else{
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("user"));
			return;
		}

		if(sAdminPassword != sPass){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(401, "Wrong password"));
			return;
		}
	}

	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM games WHERE id = :gameid");
		query.bindValue(":gameid", nGameID);
		
		if(!query.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
			return;
		}
		if(!query.next()) {
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Game not found"));	
			return;
		}
	}

	// delete from users_games
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM users_games WHERE gameid = :gameid");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query_del.lastError().text()));
			return;
		}
	}
	
	// delete from users_quests_answers
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM users_quests_answers WHERE questid IN (SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query_del.lastError().text()));
			return;
		}
	}
	
	// delete from users_quests
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM users_quests WHERE questid IN (SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query_del.lastError().text()));
			return;
		}
	}
	
	// delete from quests
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM quest WHERE gameid = :gameid");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query_del.lastError().text()));
			return;
		}
	}

	// delete from games
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM games WHERE id = :gameid");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query_del.lastError().text()));
			return;
		}
	}

    QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
