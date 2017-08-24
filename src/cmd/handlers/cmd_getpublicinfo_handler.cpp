#include "../headers/cmd_getpublicinfo_handler.h"

CmdGetPublicInfoHandler::CmdGetPublicInfoHandler(){
	
}

QString CmdGetPublicInfoHandler::cmd(){
	return "getpublicinfo";
}

bool CmdGetPublicInfoHandler::accessUnauthorized(){
	return true;
}

bool CmdGetPublicInfoHandler::accessUser(){
	return true;
}

bool CmdGetPublicInfoHandler::accessTester(){
	return true;
}

bool CmdGetPublicInfoHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdGetPublicInfoHandler::inputs(){
	return m_vInputs;
};

QString CmdGetPublicInfoHandler::description(){
	return "Method retrun public information about server";
}

QStringList CmdGetPublicInfoHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGetPublicInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["connectedusers"] = pWebSocketServer->getConnectedUsers();
	pWebSocketServer->sendMessage(pClient, jsonData);
}
