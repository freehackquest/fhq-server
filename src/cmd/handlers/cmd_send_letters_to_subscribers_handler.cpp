#include "../headers/cmd_send_letters_to_subscribers_handler.h"

CmdSendLettersToSubscribersHandler::CmdSendLettersToSubscribersHandler(){
}

QString CmdSendLettersToSubscribersHandler::cmd(){
	return "send_letters_to_subscribers";
}

bool CmdSendLettersToSubscribersHandler::accessUnauthorized(){
	return false;
}

bool CmdSendLettersToSubscribersHandler::accessUser(){
	return false;
}

bool CmdSendLettersToSubscribersHandler::accessTester(){
	return false;
}

bool CmdSendLettersToSubscribersHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdSendLettersToSubscribersHandler::inputs(){
	return m_vInputs;
};

QString CmdSendLettersToSubscribersHandler::description(){
	return "TODO: method will be send letter to subscribers";
}

QStringList CmdSendLettersToSubscribersHandler::errors(){
	QStringList	list;
	return list;
}

void CmdSendLettersToSubscribersHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	if(pUserToken == NULL){
		QJsonObject jsonData;
		jsonData["cmd"] = QJsonValue(cmd());
		jsonData["result"] = QJsonValue("FAIL");
		jsonData["error"] = QJsonValue("Not authorized request");
		pWebSocketServer->sendMessage(pClient, jsonData);
		return;
	}

	if(!pUserToken->isAdmin()){
		QJsonObject jsonData;
		jsonData["cmd"] = QJsonValue(cmd());
		jsonData["result"] = QJsonValue("FAIL");
		jsonData["error"] = QJsonValue("Allowed only fot admin");
		pWebSocketServer->sendMessage(pClient, jsonData);
		return;
	}

	// Log::info(TAG,"send_letters_to_subscribers!!!");
	
	QStringList emails;
	emails << "sea-kg@ya.ru";
	emails << "mrseakg@gmail.com";
	QString text = "Test email again";
	pWebSocketServer->sendLettersBcc(emails, "FreeHackQuest News", text);

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
