#include <cmd_hello_handler.h>

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
	return "Call this method after connect. Method return based information about server.";
}

QStringList CmdHelloHandler::errors(){
	QStringList	list;
	return list;
}

void CmdHelloHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject /*obj*/){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);

	// TODO move to another handler
	/*QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM `chatmessages` ORDER BY id DESC LIMIT 0,6");
	query.exec();
	QVector<QJsonObject> chats;
	while (query.next()) {
		QSqlRecord record = query.record();
		QJsonObject jsonChat;
		jsonChat["cmd"] = QJsonValue("chat");
		jsonChat["m"] = QJsonValue(m);
		jsonChat["type"] = QJsonValue("chat");
		jsonChat["user"] = record.value("user").toString();
		jsonChat["message"] = record.value("message").toString();
		jsonChat["dt"] = record.value("dt").toString();
		chats.push_back(jsonChat);
	}
	
	for(int i = chats.size()-1; i >= 0; i--){
		pWebSocketServer->sendMessage(pClient, chats[i]);
	}*/
}
