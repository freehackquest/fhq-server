#include <cmd_token_handler.h>
#include <runtasks.h>
#include <log.h>
#include <usertoken.h>

CmdTokenHandler::CmdTokenHandler(){
    TAG = "CmdTokenHandler";
}

QString CmdTokenHandler::cmd(){
    return "token";
}

bool CmdTokenHandler::accessUnauthorized(){
	return true;
}

bool CmdTokenHandler::accessUser(){
	return true;
}

bool CmdTokenHandler::accessTester(){
	return true;
}

bool CmdTokenHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdTokenHandler::inputs(){
	return m_vInputs;
};

QString CmdTokenHandler::description(){
    return "Method for login by token";
}

QStringList CmdTokenHandler::errors(){
	QStringList	list;
	return list;
}

void CmdTokenHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	
	if(!obj.contains("token")){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("requred parameter token"));
		return;
	}
	QString token = obj["token"].toString();
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM users_tokens WHERE token = :token");
	query.bindValue(":token", token);
	if(!query.exec()){
		Log::err(TAG, query.lastError().text());
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
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
        // TODO redesign this
        RunTasks::UpdateUserLocation(pWebSocketServer, userid, lastip);
	}else{
		Log::err(TAG, "Invalid token " + token);
		// ["error"]
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InvalidToken());
		return;
	}

	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
