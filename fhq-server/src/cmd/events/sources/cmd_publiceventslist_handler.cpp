#include <cmd_publiceventslist_handler.h>
#include <employ_database.h>
#include <QJsonArray>

CmdPublicEventsListHandler::CmdPublicEventsListHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

	// validation and description input fields
	m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
	m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

// ---------------------------------------------------------------------

std::string CmdPublicEventsListHandler::cmd(){
	return "publiceventslist";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdPublicEventsListHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdPublicEventsListHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdPublicEventsListHandler::description(){
	return
		"Return list of public events (news)\n"
		" Input params: \n"
		"   * page \n"
		"   * onpage  \n"
		"   * type  \n"
		"   * search   \n";
}

// ---------------------------------------------------------------------

void CmdPublicEventsListHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    int nPage = jsonRequest["page"].toInt();
    jsonResponse["page"] = nPage;
	
    int nOnPage = jsonRequest["onpage"].toInt();;
	if(nOnPage > 50){
        pRequest->sendMessageError(cmd(), Errors::OnPageCouldNotBeMoreThen50());
		return;
	}
    jsonResponse["onpage"] = nOnPage;

	QStringList filters;
	QMap<QString,QString> filter_values;
	
    if(jsonRequest.contains("type")){
        QString type = jsonRequest["type"].toString().trimmed();
		if(type != ""){
			filters << "(e.type = :type)";
			filter_values[":type"] = type;
		}
	}
	
    if(jsonRequest.contains("search")){
        QString search = jsonRequest["search"].toString().trimmed();
		if(search != ""){
			filters << "(e.message LIKE :search)";
			filter_values[":search"] = "%" + search + "%";
		}
	}

	QString where = filters.join(" AND "); 
	if(where.length() > 0){
		where = "WHERE " + where;
	}
	
	// count quests
    QSqlDatabase db = *(pDatabase->database());
	
	{
		QSqlQuery query(db);
		query.prepare("SELECT count(*) as cnt FROM public_events e "
			" " + where
		);
		foreach(QString key, filter_values.keys() ){
			query.bindValue(key, filter_values.value(key));
		}
		query.exec();
		if (query.next()) {
			QSqlRecord record = query.record();
            jsonResponse["count"] = record.value("cnt").toInt();
		}
	}

	// data
	QJsonArray publiceventslist;
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM public_events e"
			" " + where + 
			" ORDER BY dt DESC "
			" LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
		);
		foreach(QString key, filter_values.keys() ){
			query.bindValue(key, filter_values.value(key));
		}
		query.exec();
		while (query.next()) {
			QSqlRecord record = query.record();
			QJsonObject event;
			event["dt"] = record.value("dt").toString();
			event["id"] = record.value("id").toInt();
			event["type"] = record.value("type").toString().toHtmlEscaped(); // TODO htmlspecialchars
			event["message"] = record.value("message").toString().toHtmlEscaped(); // TODO htmlspecialchars

			publiceventslist.push_back(event);
		}
	}

    jsonResponse["data"] = publiceventslist;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
