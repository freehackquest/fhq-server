#include "../headers/cmd_deletepublicevent_handler.h"
#include <QJsonArray>

CmdDeletePublicEventHandler::CmdDeletePublicEventHandler(){
	
}

QString CmdDeletePublicEventHandler::cmd(){
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

const QVector<CmdInputDef> &CmdDeletePublicEventHandler::inputs(){
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

void CmdDeletePublicEventHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

	int nEventId = 0;
	if(obj.contains("eventid")){
		QJsonValueRef vEventId = obj["eventid"];
		if(!vEventId.isDouble()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Errors::EventIdMustBeInteger());
			return;
		}
		nEventId = vEventId.toInt();
	}else{
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::EventIdExpected());
		return;
	}
	jsonData["eventid"] = nEventId;

	QJsonObject event;
	
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM public_events WHERE id = :eventid");
	query.bindValue(":eventid", nEventId);
	query.exec();
	if (!query.next()) {
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::EventNotFound());
		return;
	}
	
	QSqlQuery query2(db);
	query2.prepare("DELETE FROM public_events WHERE id = :eventid");
	query2.bindValue(":eventid", nEventId);
	query2.exec();

	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = event;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
