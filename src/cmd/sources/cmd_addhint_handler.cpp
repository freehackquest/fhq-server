#include <cmd_addhint_handler.h>
#include <log.h>
#include <runtasks.h>

CmdAddHintHandler::CmdAddHintHandler(){
	TAG = "CmdAddHintHandler";
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

void CmdAddHintHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotAuthorizedRequest());
		return;
	}

	int questid = obj["questid"].toInt();
	if(questid == 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::QuestIDMustBeNotZero());
		return;
	}
	

	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO quests_hints (questid, text, dt) VALUES (:questid, :text, NOW())");
	query.bindValue(":questid", obj["questid"].toInt());
	query.bindValue(":text", obj["hint"].toString());
	if(!query.exec()){
		Log::err(TAG, query.lastError().text());
	}

	RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Added hint for quest #" + QString::number(questid));

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["m"] = QJsonValue(m);
	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
