#include <cmd_quest_delete_handler.h>
#include <runtasks.h>
#include <memory_cache_serverinfo.h>

CmdQuestDeleteHandler::CmdQuestDeleteHandler(){
	m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest ID"));
}

std::string CmdQuestDeleteHandler::cmd(){
    return "quest_delete";
}

bool CmdQuestDeleteHandler::accessUnauthorized(){
	return false;
}

bool CmdQuestDeleteHandler::accessUser(){
	return false;
}

bool CmdQuestDeleteHandler::accessTester(){
	return false;
}

bool CmdQuestDeleteHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdQuestDeleteHandler::inputs(){
	return m_vInputs;
};

QString CmdQuestDeleteHandler::description(){
	return "Method for delete quest";
}

QStringList CmdQuestDeleteHandler::errors(){
	QStringList	list;
	return list;
}

void CmdQuestDeleteHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
    /*IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotAuthorizedRequest());
		return;
    }*/

    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serverinfo");
    if(pMemoryCache == NULL){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
        return;
    }
    MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);

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
    pMemoryCacheServerInfo->decrementQuests();

	RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Removed quest #" + QString::number(questid) + " " + sName + " (subject: " + sSubject + ")");

	// todo recalculate rating/score for users how solved this quest

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonResponse["subject"] = sSubject;
    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}
