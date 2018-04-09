#include <cmd_user_skills_handler.h>
#include <QJsonArray>
#include <employ_database.h>

CmdUserSkillsHandler::CmdUserSkillsHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("Id of user"));
}

// ---------------------------------------------------------------------

std::string CmdUserSkillsHandler::cmd(){
	return "user_skills";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdUserSkillsHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdUserSkillsHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdUserSkillsHandler::description(){
	return "Return user skills info";
}

// ---------------------------------------------------------------------

void CmdUserSkillsHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


	QJsonObject skills_max;
	QJsonObject skills_user;

    QSqlDatabase db = *(pDatabase->database());

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
