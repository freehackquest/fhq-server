#include <cmd_deletehint_handler.h>

CmdDeleteHintHandler::CmdDeleteHintHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("hintid").required().integer_().description("hint id"));
}

// ---------------------------------------------------------------------

std::string CmdDeleteHintHandler::cmd(){
	return "deletehint";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdDeleteHintHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdDeleteHintHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdDeleteHintHandler::description(){
	return "Method for delete hint from quest";
}

// ---------------------------------------------------------------------

void CmdDeleteHintHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int hintid = jsonRequest["hintid"].toInt();
	if(hintid == 0){
        pRequest->sendMessageError(cmd(), Errors::HintIDMustBeNotZero()); // TODO redesign
		return;
	}

    QSqlDatabase db = *(pRequest->server()->database());
	QSqlQuery query(db);
	query.prepare("DELETE FROM quests_hints WHERE id = :hintid");
	query.bindValue(":hintid", hintid);
	query.exec();

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
