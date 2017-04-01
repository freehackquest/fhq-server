#include "headers/cmd_getmap_handler.h"
#include <QJsonArray>

QString CmdGetMapHandler::cmd(){
	return "getmap";
}

bool CmdGetMapHandler::accessUnauthorized(){
	return true;
}

bool CmdGetMapHandler::accessUser(){
	return true;
}

bool CmdGetMapHandler::accessTester(){
	return true;
}

bool CmdGetMapHandler::accessAdmin(){
	return true;
}

QString CmdGetMapHandler::short_description(){
	return "Returned coordinate list";
}

QString CmdGetMapHandler::description(){
	return "Returned coordinate list";
}

QStringList CmdGetMapHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGetMapHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	QJsonArray coords;
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT COUNT(*) as cnt, latitude, longitude FROM `users` GROUP BY latitude, longitude");
	query.exec();
	while (query.next()) {
		QSqlRecord record = query.record();
		double lat = record.value("latitude").toDouble();
		double lon = record.value("longitude").toDouble();
		int count = record.value("cnt").toInt();
		if(lat == 0 && lon == 0){
			continue;
		}
		QJsonObject item;
		item["lat"] = lat;
		item["lng"] = lon;
		item["count"] = count;
		coords.push_back(item);
	}

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = coords;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
