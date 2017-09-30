#include <cmd_user_skills_handler.h>
#include <QJsonArray>

CmdUserSkillsHandler::CmdUserSkillsHandler(){
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("Id of user"));
}

QString CmdUserSkillsHandler::cmd(){
	return "user_skills";
}

bool CmdUserSkillsHandler::accessUnauthorized(){
	return true;
}

bool CmdUserSkillsHandler::accessUser(){
	return true;
}

bool CmdUserSkillsHandler::accessTester(){
	return true;
}

bool CmdUserSkillsHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdUserSkillsHandler::inputs(){
	return m_vInputs;
};

QString CmdUserSkillsHandler::description(){
	return "Return user skills info";
}

QStringList CmdUserSkillsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUserSkillsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

	QJsonObject skills_max;
	QJsonObject skills_user;

	QSqlDatabase db = *(pWebSocketServer->database());
	
	{
		QSqlQuery query(db);
		query.prepare("SELECT q.subject, sum(q.score) as sum_subject FROM quest q WHERE ! ISNULL( q.subject ) GROUP BY q.subject");
		if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::DatabaseError(query.lastError().text()));
		};
		
		while(query.next()) {
			QSqlRecord record = query.record();
			QString subject = record.value("subject").toString();
			skills_max[subject] = record.value("sum_subject").toInt();
        }
	}
	
	
	int nUserID = obj["userid"].toInt();
	{
		QSqlQuery query(db);
		query.prepare("SELECT uq.userid, q.subject, SUM( q.score ) as sum_score FROM users_quests uq INNER JOIN quest q ON uq.questid = q.idquest WHERE ! ISNULL( q.subject ) AND uq.userid = :userid GROUP BY uq.userid, q.subject");
		query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::DatabaseError(query.lastError().text()));
        };
		
        while(query.next()) {
			QSqlRecord record = query.record();
			QString subject = record.value("subject").toString();
			skills_user[subject] = record.value("sum_score").toInt();
        }
	}

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["skills_max"] = skills_max;
	jsonData["skills_user"] = skills_user;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
