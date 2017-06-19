#include "../headers/cmd_addhint_handler.h"

CmdAddHintHandler::CmdAddHintHandler(){
	m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("quest id"));
	m_vInputs.push_back(CmdInputDef("hint").required().string_().description("hint text"));
}

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

const QVector<CmdInputDef> &CmdAddHintHandler::inputs(){
	return m_vInputs;
};

QString CmdAddHintHandler::description(){
	return "Methid add hint to quest";
}

QStringList CmdAddHintHandler::errors(){
	QStringList	list;
	return list;
}

void CmdAddHintHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
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

	int questid = vQuestid.toInt();
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
