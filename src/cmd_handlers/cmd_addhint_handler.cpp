#include "cmd_addhint_handler.h"

QString CmdAddHintHandler::cmd(){
	return "addhint";
}

bool CmdAddHintHandler::accessUnauthorized(){
	return false;
}

bool CmdAddHintHandler::accessUser(){
	return false;
}

bool CmdAddHintHandler::accessTester(){
	return false;
}

bool CmdAddHintHandler::accessAdmin(){
	return true;
}

QString CmdAddHintHandler::short_description(){
	return "some short description";
}

QString CmdAddHintHandler::description(){
	return "some description";
}

QStringList CmdAddHintHandler::errors(){
	QStringList	list;
	return list;
}

void CmdAddHintHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	UserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::NotAuthorizedRequest());
		return;
	}

	if(!pUserToken->isAdmin()){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::AllowedOnlyForAdmin());
		return;
	}

	QJsonValueRef vQuestid = obj["questid"];
	// bool bConvert = false;
	
	if(!vQuestid.isDouble()){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::QuestIDMustBeInteger());
		return;
	}

	int questid = vQuestid.toInt(0);
	if(questid == 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::QuestIDMustBeNotZero());
		return;
	}
	

	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO quests_hints (questid, text, dt) VALUES (:questid, :text, NOW())");
	qDebug() << "questid(n) = " << obj["questid"].toInt();
	qDebug() << "questid(s) = " << obj["questid"].toString();
	qDebug() << "hint = " << obj["hint"].toString();
	query.bindValue(":questid", obj["questid"].toInt());
	query.bindValue(":text", obj["hint"].toString());
	query.exec();

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
