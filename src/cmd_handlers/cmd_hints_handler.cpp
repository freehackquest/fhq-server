#include "headers/cmd_hints_handler.h"
#include <QJsonArray>

QString CmdHintsHandler::cmd(){
	return "hints";
}

bool CmdHintsHandler::accessUnauthorized(){
	return true;
}

bool CmdHintsHandler::accessUser(){
	return true;
}

bool CmdHintsHandler::accessTester(){
	return true;
}

bool CmdHintsHandler::accessAdmin(){
	return true;
}

QString CmdHintsHandler::short_description(){
	return "This method will be returned list of hints by questid";
}

QString CmdHintsHandler::description(){
	return "Output: list of hints, Input: questid";
}

QStringList CmdHintsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdHintsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
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

	QJsonArray hints;
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM quests_hints WHERE questid = :questid");
	query.bindValue(":questid", questid);
	query.exec();
	while (query.next()) {
		QSqlRecord record = query.record();
		int hintid = record.value("id").toInt();
		int questid2 = record.value("questid").toInt();
		QString text = record.value("text").toString();
		QString dt = record.value("dt").toString();
		QJsonObject hint;
		hint["hintid"] = hintid;
		hint["questid"] = questid2;
		hint["text"] = text;
		hint["dt"] = dt;
		hints.push_back(hint);
	}

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = hints;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
