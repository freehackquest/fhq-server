#include <cmd_handler_deletehint.h>

CmdHandlerDeleteHint::CmdHandlerDeleteHint(){
	TAG = "CmdHandlerDeleteHint";
	
    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("hintid").required().integer_().description("hint id"));
}

// ---------------------------------------------------------------------

std::string CmdHandlerDeleteHint::cmd(){
	return "deletehint";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerDeleteHint::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerDeleteHint::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerDeleteHint::description(){
	return "Method for delete hint from quest";
}

// ---------------------------------------------------------------------

void CmdHandlerDeleteHint::handle(ModelRequest *pRequest){
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
