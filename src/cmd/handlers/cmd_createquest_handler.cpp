#include "../headers/cmd_createquest_handler.h"
#include <QJsonArray>

CmdCreateQuestHandler::CmdCreateQuestHandler(){
	m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the quest"));
	m_vInputs.push_back(CmdInputDef("gameid").integer_().required().description("Which game included this quest"));
	m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the quest"));
	m_vInputs.push_back(CmdInputDef("text").string_().required().description("Description of the quest"));
	m_vInputs.push_back(CmdInputDef("score").integer_().required().description("How much append to user score after solve quest by them"));
	
	// TODO from database init
	QStringList questTypes;
	questTypes << "trivia";
	
	m_vInputs.push_back(CmdInputDef("subject").string_().required().description("Subject must be one from types (look types)"));
	m_vInputs.push_back(CmdInputDef("answer").string_().required().description("Answer for the quest"));
	m_vInputs.push_back(CmdInputDef("answer_format").string_().required().description("Answer format for the quest"));
	m_vInputs.push_back(CmdInputDef("state").string_().required().description("State of the quest, can be: open, broken, closed"));
	m_vInputs.push_back(CmdInputDef("description_state").string_().required().description("You can add some descriptions for quest state"));
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

	QString sUUID = obj["uuid"].toString().trimmed();
	int nGameID = obj["gameid"].toInt();

	QString sName = obj["name"].toString().trimmed();
	if(sName.length() == 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), Error(400, "Name could not be empty"));
		return;
	}
	
	QString sText = obj["text"].toString().trimmed();
	int nScore = obj["score"].toInt();
	QString sSubject = obj["subject"].toString().trimmed();
	QString sAnswer = obj["answer"].toString().trimmed();
	QString sAnswerFormat = obj["answer_format"].toString().trimmed();
	QString sState = obj["state"].toString().trimmed();
	QString sDescriptionState = obj["description_state"].toString().trimmed();

	
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO quest(quest_uuid,name,text,answer,score,author,subject,"
		"	answer_upper_md5,gameid,state,description_state,"
		"	count_user_solved,"
		"	copyright,answer_format,date_change,date_create)"
		"	VALUES("
		"		:quest_uuid,"
		"		:name,"
		"		:text,"
		"		:answer,"
		"		:answer_format,"
		"		:score,"
		"		:author,"
		"		:subject,"
		"		:answer_upper_md5,"
		"		:gameid,"
		"		:state,"
		"		:description_state,"
		"		:copyright,"
		"		:count_user_solved,"
		"		NOW(),"
		"		NOW()"
		"	)");
	query.bindValue(":quest_uuid", sUUID);
	query.bindValue(":name", sName);
	query.bindValue(":text", sText);
	query.bindValue(":answer", sAnswer);
	query.bindValue(":answer", sAnswer);
	query.exec();
	

	/*
	QJsonValueRef vScore = obj["score"];
	
	if(!vScore.isDouble()){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::ScoreParamMustBeInteger());
		return;
	}

	int nScore = vScore.toInt();
	if(nScore <= 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::ScoreParamMustBeMoreZero());
		return;
	}*/

	// check enum

	// TODO insert quest
/*
	

	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);*/
}
