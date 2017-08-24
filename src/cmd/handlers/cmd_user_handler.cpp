#include "../headers/cmd_user_handler.h"
#include <QJsonArray>

CmdUserHandler::CmdUserHandler(){
	m_vInputs.push_back(CmdInputDef("userid").optional().integer_().description("Id of user"));
}

QString CmdUserHandler::cmd(){
	return "user";
}

bool CmdUserHandler::accessUnauthorized(){
	return true;
}

bool CmdUserHandler::accessUser(){
	return true;
}

bool CmdUserHandler::accessTester(){
	return true;
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
	return list;
}

void CmdUserHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(!obj.contains("userid") && pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotAuthorizedRequest());
		return;
	}

	bool bCurrentUserOrAdmin = false;
	
	int nUserID = 0;
	if(pUserToken != NULL){
		nUserID = pUserToken->userid();
		bCurrentUserOrAdmin = true;
	}

	if(obj.contains("userid")){
		nUserID = obj["userid"].toInt();
		bCurrentUserOrAdmin = false;
	}
	
	QJsonObject user;
	QJsonObject profile;
	QSqlDatabase db = *(pWebSocketServer->database());
	
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM users WHERE id = :userid");
		query.bindValue(":userid", nUserID);
		query.exec();
		
		if (query.next()) {
			QSqlRecord record = query.record();
			user["id"] = record.value("id").toInt();
			user["uuid"] = record.value("uuid").toString();
			user["nick"] = record.value("nick").toString();
			user["role"] = record.value("role").toString();
			user["logo"] = record.value("logo").toString();
			user["status"] = record.value("status").toString();
			user["rating"] = record.value("rating").toString();
			
			if(bCurrentUserOrAdmin){
				user["email"] = record.value("email").toString();
				user["dt_create"] = record.value("dt_create").toString();
				user["dt_last_login"] = record.value("dt_last_login").toString();
				user["last_ip"] = record.value("last_ip").toString();
				user["country"] = record.value("country").toString();
				user["region"] = record.value("region").toString();
				user["city"] = record.value("city").toString();
			}
		}else{
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("user"));
			return;
		}
	}
	
	{
		QSqlQuery query(db);
		query.prepare("SELECT name, value, date_change FROM users_profile WHERE userid = :userid");
		query.bindValue(":userid", nUserID);
		query.exec();
		
		while (query.next()) {
			QSqlRecord record = query.record();
			QString name = record.value("name").toString();
			QString value = record.value("value").toString();
			profile[name] = value;
			
			// TODO clenup 'template' from user profiles
		}
	}
	

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["data"] = user;
	jsonData["profile"] = profile;
	jsonData["access"] = bCurrentUserOrAdmin;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
