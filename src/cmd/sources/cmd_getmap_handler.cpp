#include <cmd_getmap_handler.h>
#include <QJsonArray>
#include <employ_settings.h>

CmdGetMapHandler::CmdGetMapHandler(){
	
}

std::string CmdGetMapHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdGetMapHandler::inputs(){
	return m_vInputs;
};

QString CmdGetMapHandler::description(){
	return "Returned coordinate list";
}

QStringList CmdGetMapHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGetMapHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject /*obj*/){

    EmploySettings *pSettings = findEmploy<EmploySettings>();

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
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
	jsonData["data"] = coords;
    jsonData["google_map_api_key"] = pSettings->getSettString("google_map_api_key");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
