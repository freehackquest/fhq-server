#include <cmd_deletehint_handler.h>

CmdDeleteHintHandler::CmdDeleteHintHandler(){
	m_vInputs.push_back(CmdInputDef("hintid").required().integer_().description("hint id"));
}

std::string CmdDeleteHintHandler::cmd(){
	return "deletehint";
}

bool CmdDeleteHintHandler::accessUnauthorized(){
	return false;
}

bool CmdDeleteHintHandler::accessUser(){
	return false;
}

bool CmdDeleteHintHandler::accessTester(){
	return false;
}

bool CmdDeleteHintHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdDeleteHintHandler::inputs(){
	return m_vInputs;
};

QString CmdDeleteHintHandler::description(){
	return "Method for delete hint from quest";
}

QStringList CmdDeleteHintHandler::errors(){
	QStringList	list;
	return list;
}

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
