#include "../headers/cmd_hello_handler.h"

CmdHelloHandler::CmdHelloHandler(){
}

QString CmdHelloHandler::cmd(){
	return "hello";
}

bool CmdHelloHandler::accessUnauthorized(){
	return true;
}

bool CmdHelloHandler::accessUser(){
	return true;
}

bool CmdHelloHandler::accessTester(){
	return true;
}

bool CmdHelloHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdHelloHandler::inputs(){
	return m_vInputs;
};

QString CmdHelloHandler::description(){
	return "some description";
}

QStringList CmdHelloHandler::errors(){
	QStringList	list;
	return list;
}

void CmdHelloHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject /*obj*/){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	pWebSocketServer->sendMessage(pClient, jsonData);

	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM `chatmessages` ORDER BY id DESC LIMIT 0,6");
	query.exec();
	QVector<QJsonObject> chats;
	while (query.next()) {
		QSqlRecord record = query.record();
		QJsonObject jsonChat;
		jsonChat["cmd"] = QJsonValue("chat");
		jsonChat["type"] = QJsonValue("chat");
		jsonChat["user"] = record.value("user").toString();
		jsonChat["message"] = record.value("message").toString();
		jsonChat["dt"] = record.value("dt").toString();
		chats.push_back(jsonChat);
	}
	
	for(int i = chats.size()-1; i >= 0; i--){
		pWebSocketServer->sendMessage(pClient, chats[i]);
	}
	
}
