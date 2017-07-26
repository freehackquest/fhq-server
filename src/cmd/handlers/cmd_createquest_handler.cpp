#include "../headers/cmd_createquest_handler.h"
#include <QJsonArray>

CmdCreateQuestHandler::CmdCreateQuestHandler(){
	m_vInputs.push_back(CmdInputDef("uuid").string_().required());
	m_vInputs.push_back(CmdInputDef("gameid").integer_().required());
	m_vInputs.push_back(CmdInputDef("name").string_().required());
	m_vInputs.push_back(CmdInputDef("text").string_().required());
	m_vInputs.push_back(CmdInputDef("score").integer_().required());
	m_vInputs.push_back(CmdInputDef("subject").string_().required());
	m_vInputs.push_back(CmdInputDef("answer").string_().required());
	m_vInputs.push_back(CmdInputDef("answer_format").string_().required());
	m_vInputs.push_back(CmdInputDef("state").string_().required());
	m_vInputs.push_back(CmdInputDef("description_state").string_().required());
}

QString CmdCreateQuestHandler::cmd(){
	return "createquest";
}

bool CmdCreateQuestHandler::accessUnauthorized(){
	return false;
}

bool CmdCreateQuestHandler::accessUser(){
	return false;
}

bool CmdCreateQuestHandler::accessTester(){
	return false;
}

bool CmdCreateQuestHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdCreateQuestHandler::inputs(){
	return m_vInputs;
};

QString CmdCreateQuestHandler::description(){
	return "Create the quest";
}

QStringList CmdCreateQuestHandler::errors(){
	QStringList	list;
	return list;
}

void CmdCreateQuestHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

	if(!obj.contains("uuid")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::UUIDParamExpected());
		return;
	}
	
	if(!obj.contains("gameid")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::GameIDParamExpected());
		return;
	}
	
	if(!obj.contains("name")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::NameParamExpected());
		return;
	}
	
	if(!obj.contains("text")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::TextParamExpected());
		return;
	}
	
	if(!obj.contains("score")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::ScoreParamExpected());
		return;
	}
	
	if(!obj.contains("subject")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::SubjectParamExpected());
		return;
	}
	
	if(!obj.contains("answer")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::AnswerParamExpected());
		return;
	}
	
	if(!obj.contains("answer_format")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::AnswerFormatParamExpected());
		return;
	}
	
	if(!obj.contains("state")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::StateParamExpected());
		return;
	}

	if(!obj.contains("description_state")){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::DescriptionStateParamExpected());
		return;
	}

	// TODO insert quest
/*
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO public_events(type,message,dt) VALUES(:type,:message,NOW())");
	query.bindValue(":type", type);
	query.bindValue(":message", message);
	query.exec();

	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);*/
}
