#include "cmd_login_handler.h"
#include "../tasks/update_user_location_task.h"
#include <QThreadPool>

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

QString CmdLoginHandler::short_description(){
	return "some short description";
}

QString CmdLoginHandler::description(){
	return "some description";
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

	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM users_tokens WHERE token = :token");
	query.bindValue(":token", obj["token"].toString());
	query.exec();
	if (query.next()) {
		QSqlRecord record = query.record();
		int userid = record.value("userid").toInt();
		QString status = record.value("status").toString();
		QString data = record.value("data").toString();
		QString start_date = record.value("start_date").toString();
		QString end_date = record.value("end_date").toString();
		qDebug() << "userid " << userid;
		qDebug() << "status " << status;
		qDebug() << "data " << data;
		qDebug() << "start_date " << start_date;
		qDebug() << "end_date " << end_date;
		QString lastip = pClient->peerAddress().toString();
		pWebSocketServer->setUserToken(pClient, new UserToken(data));
		UpdateUserLocationTask *pUpdateUserLocationTask = new UpdateUserLocationTask(pWebSocketServer, userid, lastip);
		QThreadPool::globalInstance()->start(pUpdateUserLocationTask);
		
	}else{
		jsonData["result"] = QJsonValue("FAIL");
		jsonData["error"] = QJsonValue("Invalid token");
		pWebSocketServer->sendMessage(pClient, jsonData);
		return;
	}

	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
