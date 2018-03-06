#include <cmd_classbook_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookHandler::CmdClassbookHandler(){
}

std::string CmdClassbookHandler::cmd(){
	return "classbook";
}

bool CmdClassbookHandler::accessUnauthorized(){
	return true;
}

bool CmdClassbookHandler::accessUser(){
	return true;
}

bool CmdClassbookHandler::accessTester(){
	return true;
}

bool CmdClassbookHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdClassbookHandler::inputs(){
	return m_vInputs;
};

QString CmdClassbookHandler::description(){
	return "Return classbook contents";
}

QStringList CmdClassbookHandler::errors(){
	QStringList	list;
	return list;
}

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
