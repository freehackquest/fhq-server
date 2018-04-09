#include <cmd_handler_quests_subjects.h>
#include <QJsonArray>
#include <employ_database.h>

CmdHandlerQuestsSubjects::CmdHandlerQuestsSubjects(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	// m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
	// m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestsSubjects::cmd(){
    return "quests_subjects";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerQuestsSubjects::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerQuestsSubjects::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestsSubjects::description(){
	return "Method returned list of games";
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsSubjects::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QJsonArray subjects;
    QSqlDatabase db = *(pDatabase->database());

	QSqlQuery query(db);
    query.prepare("SELECT subject, COUNT(*) as cnt FROM `quest` WHERE quest.state = :state GROUP BY subject");
    query.bindValue(":state", "open");

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

	while (query.next()) {
		QSqlRecord record = query.record();
        QJsonObject subject;
        subject["subject"] = record.value("subject").toString();
        subject["count"] = record.value("cnt").toInt();
        subjects.push_back(subject);
	}

    jsonResponse["data"] = subjects;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);

}
