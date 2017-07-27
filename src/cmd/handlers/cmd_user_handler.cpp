#include "../headers/cmd_user_handler.h"
#include <QJsonArray>

CmdUserHandler::CmdUserHandler(){
	m_vInputs.push_back(CmdInputDef("uuid").required().string_().description("UUID of user"));
}

QString CmdUserHandler::cmd(){
	return "user";
}

bool CmdUserHandler::accessUnauthorized(){
	return false;
}

bool CmdUserHandler::accessUser(){
	return false;
}

bool CmdUserHandler::accessTester(){
	return false;
}

bool CmdUserHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdUserHandler::inputs(){
	return m_vInputs;
};

QString CmdUserHandler::description(){
	return "Return user info";
}

QStringList CmdUserHandler::errors(){
	QStringList	list;
	list << Errors::NotAuthorizedRequest().message();
	list << Errors::NotFoundUserByUUID("some uuid").message();
	return list;
}

void CmdUserHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::NotAuthorizedRequest());
		return;
	}

	QString uuid = obj["uuid"].toString();
	
	QJsonObject user;
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM users WHERE uuid = :uuid");
	query.bindValue(":uuid", uuid);
	query.exec();
	if (query.next()) {
		QSqlRecord record = query.record();
		user["id"] = record.value("id").toInt();
		user["uuid"] = record.value("uuid").toString();
		user["email"] = record.value("email").toString();
		user["nick"] = record.value("nick").toString();
		user["role"] = record.value("role").toString();
		user["logo"] = record.value("logo").toString();
		user["dt_create"] = record.value("dt_create").toString();
		user["dt_last_login"] = record.value("dt_last_login").toString();
		user["last_ip"] = record.value("last_ip").toString();
		user["status"] = record.value("status").toString();
		
		QSqlQuery query_profile(db);
		query_profile.prepare("SELECT users_profile.name, users_profile.value, users_profile.date_change FROM users_profile INNER JOIN users ON users_profile.userid = users.id WHERE uuid = :uuid");
		query_profile.bindValue(":uuid", uuid);
		query_profile.exec();
		QJsonObject user_profile;
		while (query_profile.next()) {
			QSqlRecord record = query_profile.record();
			QString name = record.value("name").toString();
			QString value = record.value("value").toString();
			user_profile[name] = value;
		}
		user["profile"] = user_profile;
	}else{
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::NotFoundUserByUUID(uuid));
		return;
	}

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = user;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
