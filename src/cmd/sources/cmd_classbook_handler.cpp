#include <cmd_classbook_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookHandler::CmdClassbookHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

}

// ---------------------------------------------------------------------

std::string CmdClassbookHandler::cmd(){
	return "classbook";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdClassbookHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdClassbookHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdClassbookHandler::description(){
	return "Return classbook contents";
}

// ---------------------------------------------------------------------

void CmdClassbookHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

	QJsonArray contents;

	QSqlQuery query(db);
	query.prepare("SELECT * FROM classbook ORDER BY id ASC");
	query.exec();
	while (query.next()) {
		QSqlRecord record = query.record();
		QString uuid = record.value("uuid").toString();
		QJsonObject jsonItem;
		jsonItem["id"] = record.value("id").toInt();
		jsonItem["parentid"] = record.value("parentid").toInt();
		jsonItem["uuid"] = uuid;
		jsonItem["parentuuid"] = record.value("parentuuid").toString();
		jsonItem["name_ru"] = record.value("name_ru").toString();
		jsonItem["name_en"] = record.value("name_en").toString();
		jsonItem["name"] = record.value("name_en").toString();
		jsonItem["link"] = QJsonValue("files/classbook/" + uuid + "_en.md");
		contents.push_back(jsonItem);
	}

    jsonResponse["items"] = contents;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
