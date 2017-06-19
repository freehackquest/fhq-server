#include "../headers/cmd_api_handler.h"

QString CmdApiHandler::cmd(){
	return "api";
}

bool CmdApiHandler::accessUnauthorized(){
	return true;
}

bool CmdApiHandler::accessUser(){
	return true;
}

bool CmdApiHandler::accessTester(){
	return true;
}

bool CmdApiHandler::accessAdmin(){
	return true;
}

QString CmdApiHandler::short_description(){
	return "some short description";
}

QString CmdApiHandler::description(){
	return "some description";
}

QStringList CmdApiHandler::errors(){
	QStringList	list;
	return list;
}

void CmdApiHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	QJsonObject data;
	pWebSocketServer->exportApi(data);
	jsonData["data"] = data;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
