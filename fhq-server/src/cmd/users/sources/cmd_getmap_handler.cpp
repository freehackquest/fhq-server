#include <cmd_getmap_handler.h>
#include <QJsonArray>
#include <employ_settings.h>
#include <employ_database.h>

CmdGetMapHandler::CmdGetMapHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

std::string CmdGetMapHandler::cmd(){
	return "getmap";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGetMapHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGetMapHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGetMapHandler::description(){
	return "Returned coordinate list";
}

// ---------------------------------------------------------------------

void CmdGetMapHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

	QJsonArray coords;
    QSqlDatabase db = *(pDatabase->database());
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

    jsonResponse["data"] = coords;
    jsonResponse["google_map_api_key"] = pSettings->getSettString("google_map_api_key");
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
