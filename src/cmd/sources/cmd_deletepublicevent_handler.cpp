#include <cmd_deletepublicevent_handler.h>
#include <QJsonArray>

CmdDeletePublicEventHandler::CmdDeletePublicEventHandler(){
	m_vInputs.push_back(CmdInputDef("hintid").required().integer_().description("hint id"));
	
}

std::string CmdDeletePublicEventHandler::cmd(){
	return "deletepublicevent";
}

bool CmdDeletePublicEventHandler::accessUnauthorized(){
	return false;
}

bool CmdDeletePublicEventHandler::accessUser(){
	return false;
}

bool CmdDeletePublicEventHandler::accessTester(){
	return false;
}

bool CmdDeletePublicEventHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdDeletePublicEventHandler::inputs(){
	return m_vInputs;
};

QString CmdDeletePublicEventHandler::description(){
	return
		"Delete public event\n"
		" Input params: \n"
		"   * eventid \n";
}

QStringList CmdDeletePublicEventHandler::errors(){
	QStringList	list;
	return list;
}

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
