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

const std::vector<CmdInputDef> &CmdSendChatMessageHandler::inputs(){
	return m_vInputs;
};

QString CmdSendChatMessageHandler::description(){
	return "Method will be keep message and it sent to another users";
}

QStringList CmdSendChatMessageHandler::errors(){
	QStringList	list;
	return list;
}

void CmdSendChatMessageHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    IUserToken *pUserToken = pRequest->userToken();
	QString username = "";
	if(pUserToken != NULL){
		username = pUserToken->nick();
	}else{
		username = "Guest";
	}

    pRequest->sendMessageSuccess(cmd(), jsonResponse);

    QSqlDatabase db = *(pRequest->server()->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO chatmessages(user, message, dt) VALUES(:user,:message, NOW())");
	query.bindValue(":user", username);
    query.bindValue(":message", jsonRequest["message"].toString());
	query.exec();
	
	QJsonObject jsonData2;
	jsonData2["cmd"] = QJsonValue("chat");
    jsonData2["type"] = jsonRequest["type"];
	jsonData2["user"] = username;
    jsonData2["message"] = jsonRequest["message"];
	jsonData2["dt"] = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");

    pRequest->server()->sendToAll(jsonData2);
}
