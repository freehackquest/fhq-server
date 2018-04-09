#include <cmd_handler_addhint.h>
#include <log.h>
#include <runtasks.h>
#include <employ_database.h>

CmdHandlerAddHint::CmdHandlerAddHint(){
	TAG = "CmdHandlerAddHint";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("quest id"));
	m_vInputs.push_back(CmdInputDef("hint").required().string_().description("hint text"));
}

// ---------------------------------------------------------------------

std::string CmdHandlerAddHint::cmd(){
	return "addhint";
}

// ---------------------------------------------------------------------

std::string CmdHandlerAddHint::description(){
    return "Methid add hint to quest";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerAddHint::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerAddHint::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

void CmdHandlerAddHint::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();
    QJsonObject jsonResponse; // TODO redesign to nlohmann::json

    // get quest id
    int nQuestId = 0;
    if(jsonRequest["questid"].is_number_integer()){
        nQuestId = jsonRequest.at("questid");
    }

    if(nQuestId == 0){
        // todo this check move to cmd input def
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
		return;
	}

    // hint text
    std::string sHint = "";
    if(jsonRequest["hint"].is_string()){
        sHint = jsonRequest["hint"];
    }

    QSqlDatabase db = *(pDatabase->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO quests_hints (questid, text, dt) VALUES (:questid, :text, NOW())");
    query.bindValue(":questid", nQuestId);
    query.bindValue(":text", QString(sHint.c_str()));
	if(!query.exec()){
		Log::err(TAG, query.lastError().text());
	}

    RunTasks::AddPublicEvents("quests", "Added hint for quest #" + QString::number(nQuestId));
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
