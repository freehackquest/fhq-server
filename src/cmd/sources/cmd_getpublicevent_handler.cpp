#include <cmd_getpublicevent_handler.h>
#include <QJsonArray>

CmdGetPublicEventHandler::CmdGetPublicEventHandler(){
	m_vInputs.push_back(CmdInputDef("eventid").required().integer_().description("Event id"));
}

std::string CmdGetPublicEventHandler::cmd(){
	return "getpublicevent";
}

bool CmdGetPublicEventHandler::accessUnauthorized(){
	return true;
}

bool CmdGetPublicEventHandler::accessUser(){
	return true;
}

bool CmdGetPublicEventHandler::accessTester(){
	return true;
}

bool CmdGetPublicEventHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdGetPublicEventHandler::inputs(){
	return m_vInputs;
};

QString CmdGetPublicEventHandler::description(){
	return
		"Return public event info by id\n"
		" Input params: \n"
		"   * eventid \n";
}

QStringList CmdGetPublicEventHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGetPublicEventHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int nEventId = jsonRequest["eventid"].toInt();
    jsonResponse["eventid"] = nEventId;

	QJsonObject event;
	
    QSqlDatabase db = *(pRequest->server()->database());
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
