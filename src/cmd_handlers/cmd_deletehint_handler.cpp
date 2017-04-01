#include "headers/cmd_deletehint_handler.h"

QString CmdDeleteHintHandler::cmd(){
	return "deletehint";
}

bool CmdDeleteHintHandler::accessUnauthorized(){
	return false;
}

bool CmdDeleteHintHandler::accessUser(){
	return false;
}

bool CmdDeleteHintHandler::accessTester(){
	return false;
}

bool CmdDeleteHintHandler::accessAdmin(){
	return true;
}

QString CmdDeleteHintHandler::short_description(){
	return "some short description";
}

QString CmdDeleteHintHandler::description(){
	return "some description";
}

QStringList CmdDeleteHintHandler::errors(){
	QStringList	list;
	return list;
}

void CmdDeleteHintHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	UserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::NotAuthorizedRequest());
		return;
	}

	if(!pUserToken->isAdmin()){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::AllowedOnlyForAdmin());
		return;
	}

	QJsonValueRef vHintid = obj["hintid"];
	
	if(!vHintid.isDouble()){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::HintIDMustBeInteger());
		return;
	}

	int hintid = vHintid.toInt(0);
	if(hintid == 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::HintIDMustBeNotZero());
		return;
	}
	

	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("DELETE FROM quests_hints WHERE id = :hintid");
	query.bindValue(":hintid", hintid);
	query.exec();

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
