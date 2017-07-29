#include "../headers/cmd_login_handler.h"
#include <runtasks.h>
#include <log.h>
#include <usertoken.h>

CmdLoginHandler::CmdLoginHandler(){
	TAG = "CmdLoginHandler";
}

QString CmdLoginHandler::cmd(){
	return "login";
}

bool CmdLoginHandler::accessUnauthorized(){
	return true;
}

bool CmdLoginHandler::accessUser(){
	return true;
}

bool CmdLoginHandler::accessTester(){
	return true;
}

bool CmdLoginHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdLoginHandler::inputs(){
	return m_vInputs;
};

QString CmdLoginHandler::description(){
	return "Method for login";
}

QStringList CmdLoginHandler::errors(){
	QStringList	list;
	return list;
}

void CmdLoginHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	
	if(!obj.contains("token")){
		jsonData["result"] = QJsonValue("FAIL");
		jsonData["error"] = QJsonValue("Not found requred parameter token");
		pWebSocketServer->sendMessage(pClient, jsonData);
		return;
	}
	QString token = obj["token"].toString();
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM users_tokens WHERE token = :token");
	query.bindValue(":token", token);
	if(!query.exec()){
		Log::err(TAG, query.lastError().text());
		pWebSocketServer->sendMessageError(pClient, cmd(), Error(500, query.lastError().text()));
		return;
	}
	if (query.next()) {
		QSqlRecord record = query.record();
		int userid = record.value("userid").toInt();
		QString status = record.value("status").toString();
		QString data = record.value("data").toString();
		QString start_date = record.value("start_date").toString();
		QString end_date = record.value("end_date").toString();
		QString lastip = pClient->peerAddress().toString();
		pWebSocketServer->setUserToken(pClient, new UserToken(data));
		RunTasks::UpdateUserLocation(pWebSocketServer, userid, lastip);

	}else{
		Log::err(TAG, "Invalid token " + token);
		// ["error"]
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::InvalidToken());
		return;
	}

	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
