#include <cmd_getpublicevent_handler.h>
#include <employ_database.h>
#include <QJsonArray>

CmdGetPublicEventHandler::CmdGetPublicEventHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("eventid").required().integer_().description("Event id"));
}

// ---------------------------------------------------------------------

std::string CmdGetPublicEventHandler::cmd(){
	return "getpublicevent";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGetPublicEventHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGetPublicEventHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGetPublicEventHandler::description(){
	return
		"Return public event info by id\n"
		" Input params: \n"
		"   * eventid \n";
}

// ---------------------------------------------------------------------

void CmdGetPublicEventHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int nEventId = jsonRequest["eventid"].toInt();
    jsonResponse["eventid"] = nEventId;

	QJsonObject event;
	
    QSqlDatabase db = *(pDatabase->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM public_events e WHERE id = :eventid");
	query.bindValue(":eventid", nEventId);
	query.exec();
	if (query.next()) {
		QSqlRecord record = query.record();
		event["dt"] = record.value("dt").toString();
		event["id"] = record.value("id").toInt();
		event["type"] = record.value("type").toString().toHtmlEscaped(); // TODO htmlspecialchars
		event["message"] = record.value("message").toString().toHtmlEscaped(); // TODO htmlspecialchars
	}else{
        pRequest->sendMessageError(cmd(), Errors::EventNotFound());
		return;
	}

    jsonResponse["data"] = event;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
