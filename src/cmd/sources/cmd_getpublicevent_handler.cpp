#include <cmd_getpublicevent_handler.h>
#include <QJsonArray>

CmdGetPublicEventHandler::CmdGetPublicEventHandler(){
	m_vInputs.push_back(CmdInputDef("eventid").required().integer_().description("Event id"));
}

QString CmdGetPublicEventHandler::cmd(){
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

const QVector<CmdInputDef> &CmdGetPublicEventHandler::inputs(){
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

void CmdGetPublicEventHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

	int nEventId = obj["eventid"].toInt();
	jsonData["eventid"] = nEventId;

	QJsonObject event;
	
	QSqlDatabase db = *(pWebSocketServer->database());
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
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::EventNotFound());
		return;
	}

	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["data"] = event;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
