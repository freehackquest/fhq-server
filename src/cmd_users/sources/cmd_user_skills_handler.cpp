#include <cmd_user_skills_handler.h>
#include <QJsonArray>

CmdUserSkillsHandler::CmdUserSkillsHandler(){
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("Id of user"));
}

std::string CmdUserSkillsHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdUserSkillsHandler::inputs(){
	return m_vInputs;
};

QString CmdUserSkillsHandler::description(){
	return "Return user skills info";
}

QStringList CmdUserSkillsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUserSkillsHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


	QJsonObject skills_max;
	QJsonObject skills_user;

    QSqlDatabase db = *(pRequest->server()->database());

	{
		QSqlQuery query(db);
		query.prepare("SELECT q.subject, sum(q.score) as sum_subject FROM quest q WHERE ! ISNULL( q.subject ) GROUP BY q.subject");
		if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
		};
		
		while(query.next()) {
			QSqlRecord record = query.record();
			QString subject = record.value("subject").toString();
			skills_max[subject] = record.value("sum_subject").toInt();
        }
	}
	
	
    int nUserID = jsonRequest["userid"].toInt();
	{
		QSqlQuery query(db);
		query.prepare("SELECT uq.userid, q.subject, SUM( q.score ) as sum_score FROM users_quests uq INNER JOIN quest q ON uq.questid = q.idquest WHERE ! ISNULL( q.subject ) AND uq.userid = :userid GROUP BY uq.userid, q.subject");
		query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
        };
		
        while(query.next()) {
			QSqlRecord record = query.record();
			QString subject = record.value("subject").toString();
			skills_user[subject] = record.value("sum_score").toInt();
        }
	}

    jsonResponse["skills_max"] = skills_max;
    jsonResponse["skills_user"] = skills_user;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
