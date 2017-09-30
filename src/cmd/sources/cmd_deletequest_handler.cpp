#include <cmd_deletequest_handler.h>
#include <runtasks.h>


CmdDeleteQuestHandler::CmdDeleteQuestHandler(){
	m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest ID"));
}

QString CmdDeleteQuestHandler::cmd(){
	return "deletequest";
}

bool CmdDeleteQuestHandler::accessUnauthorized(){
	return false;
}

bool CmdDeleteQuestHandler::accessUser(){
	return false;
}

bool CmdDeleteQuestHandler::accessTester(){
	return false;
}

bool CmdDeleteQuestHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdDeleteQuestHandler::inputs(){
	return m_vInputs;
};

QString CmdDeleteQuestHandler::description(){
	return "Method for delete quest";
}

QStringList CmdDeleteQuestHandler::errors(){
	QStringList	list;
	return list;
}

void CmdDeleteQuestHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotAuthorizedRequest());
		return;
	}

	int questid = obj["questid"].toInt();
	QString sName = "";
	QString sSubject = "";
	QSqlDatabase db = *(pWebSocketServer->database());
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM quest WHERE idquest = :questid");
		query.bindValue(":questid", questid);
		if(!query.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
			return;
		}
		if (query.next()) {
			QSqlRecord record = query.record();
			sName = record.value("name").toString();
			sSubject = record.value("subject").toString();
		}else{
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Quest not found"));
			return;
		}
	}
	
	{
		QSqlQuery query(db);
		query.prepare("DELETE FROM quest WHERE idquest = :questid");
		query.bindValue(":questid", questid);
		if(!query.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
			return;
		}
	}

	// remove from users_quests_answers
	{
		QSqlQuery query(db);
		query.prepare("DELETE FROM users_quests_answers WHERE questid = :questid");
		query.bindValue(":questid", questid);
		if(!query.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
			return;
		}
	}

	// remove from users_quests
	{
		QSqlQuery query(db);
		query.prepare("DELETE FROM users_quests WHERE questid = :questid");
		query.bindValue(":questid", questid);
		if(!query.exec()){
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
			return;
		}
	}

	RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Removed quest #" + QString::number(questid) + " " + sName + " (subject: " + sSubject + ")");

	// todo recalculate rating/score for users how solved this quest

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["subject"] = sSubject;
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
