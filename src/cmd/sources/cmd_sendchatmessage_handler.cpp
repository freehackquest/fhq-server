#include <cmd_sendchatmessage_handler.h>

CmdSendChatMessageHandler::CmdSendChatMessageHandler(){
}

std::string CmdSendChatMessageHandler::cmd(){
	return "sendchatmessage";
}

bool CmdSendChatMessageHandler::accessUnauthorized(){
	return true;
}

bool CmdSendChatMessageHandler::accessUser(){
	return true;
}

bool CmdSendChatMessageHandler::accessTester(){
	return true;
}

bool CmdSendChatMessageHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdSendChatMessageHandler::inputs(){
	return m_vInputs;
};

QString CmdSendChatMessageHandler::description(){
	return "Method will be keep message and it sent to another users";
}

QStringList CmdSendChatMessageHandler::errors(){
	QStringList	list;
	return list;
}

void CmdSendChatMessageHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m,  QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	QString username = "";
	if(pUserToken != NULL){
		username = pUserToken->nick();
	}else{
		username = "Guest";
	}

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
	
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO chatmessages(user, message, dt) VALUES(:user,:message, NOW())");
	query.bindValue(":user", username);
	query.bindValue(":message", obj["message"].toString());
	query.exec();
	
	QJsonObject jsonData2;
	jsonData2["cmd"] = QJsonValue("chat");
	jsonData2["type"] = obj["type"];
	jsonData2["user"] = username;
	jsonData2["message"] = obj["message"];
	jsonData2["dt"] = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");

	pWebSocketServer->sendToAll(jsonData2);
	
	
}
