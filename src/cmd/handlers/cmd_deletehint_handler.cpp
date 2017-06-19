#include "../headers/cmd_deletehint_handler.h"

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

const QVector<CmdInputDef> &CmdDeleteHintHandler::inputs(){
	return m_vInputs;
};

QString CmdDeleteHintHandler::description(){
	return "some description";
}

QStringList CmdDeleteHintHandler::errors(){
	QStringList	list;
	return list;
}

void CmdDeleteHintHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
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

	int hintid = vHintid.toInt();
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
