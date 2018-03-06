#include <cmd_deletepublicevent_handler.h>
#include <QJsonArray>

CmdDeletePublicEventHandler::CmdDeletePublicEventHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("hintid").required().integer_().description("hint id"));
	
}

// ---------------------------------------------------------------------

std::string CmdDeletePublicEventHandler::cmd(){
	return "deletepublicevent";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdDeletePublicEventHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdDeletePublicEventHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdDeletePublicEventHandler::description(){
	return
		"Delete public event\n"
		" Input params: \n"
		"   * eventid \n";
}

// ---------------------------------------------------------------------

void CmdDeletePublicEventHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));

    int nEventId = jsonRequest["eventid"].toInt();
	jsonData["eventid"] = nEventId;

	QJsonObject event;
	
    QSqlDatabase db = *(pRequest->server()->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM public_events WHERE id = :eventid");
	query.bindValue(":eventid", nEventId);
	query.exec();
	if (!query.next()) {
        pRequest->sendMessageError(cmd(), Errors::EventNotFound());
		return;
	}
	
	QSqlQuery query2(db);
	query2.prepare("DELETE FROM public_events WHERE id = :eventid");
	query2.bindValue(":eventid", nEventId);
	query2.exec();

	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = event;
    pRequest->sendMessageSuccess(cmd(), jsonData);
}
