#include "../headers/cmd_createpublicevent_handler.h"
#include <QJsonArray>

CmdCreatePublicEventHandler::CmdCreatePublicEventHandler(){
	QStringList eventTypes;
	// TODO load from database
	eventTypes << "info";
	eventTypes << "users";
	eventTypes << "games";
	eventTypes << "quests";
	eventTypes << "warning";
	
	m_vInputs.push_back(CmdInputDef("type").enum_(eventTypes).required());
	m_vInputs.push_back(CmdInputDef("message").string_().required());
}

QString CmdCreatePublicEventHandler::cmd(){
	return "createpublicevent";
}

bool CmdCreatePublicEventHandler::accessUnauthorized(){
	return false;
}

bool CmdCreatePublicEventHandler::accessUser(){
	return false;
}

bool CmdCreatePublicEventHandler::accessTester(){
	return false;
}

bool CmdCreatePublicEventHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdCreatePublicEventHandler::inputs(){
	return m_vInputs;
};

QString CmdCreatePublicEventHandler::description(){
	return "Create the public event";
}

QStringList CmdCreatePublicEventHandler::errors(){
	QStringList	list;
	return list;
}

void CmdCreatePublicEventHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

	QString type = obj["type"].toString().trimmed();
	QString message = obj["message"].toString().trimmed();
	

	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO public_events(type,message,dt) VALUES(:type,:message,NOW())");
	query.bindValue(":type", type);
	query.bindValue(":message", message);
	query.exec();

	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
