#include <cmd_publiceventslist_handler.h>
#include <QJsonArray>

CmdPublicEventsListHandler::CmdPublicEventsListHandler(){
	// validation and description input fields
	m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
	m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

QString CmdPublicEventsListHandler::cmd(){
	return "publiceventslist";
}

bool CmdPublicEventsListHandler::accessUnauthorized(){
	return true;
}

bool CmdPublicEventsListHandler::accessUser(){
	return true;
}

bool CmdPublicEventsListHandler::accessTester(){
	return true;
}

bool CmdPublicEventsListHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdPublicEventsListHandler::inputs(){
	return m_vInputs;
};

QString CmdPublicEventsListHandler::description(){
	return
		"Return list of public events (news)\n"
		" Input params: \n"
		"   * page \n"
		"   * onpage  \n"
		"   * type  \n"
		"   * search   \n";
}

QStringList CmdPublicEventsListHandler::errors(){
	QStringList	list;
	return list;
}

void CmdPublicEventsListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

	int nPage = obj["page"].toInt();
	jsonData["page"] = nPage;
	
	int nOnPage = obj["onpage"].toInt();;
	if(nOnPage > 50){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::OnPageCouldNotBeMoreThen50());
		return;
	}
	jsonData["onpage"] = nOnPage;

	QStringList filters;
	QMap<QString,QString> filter_values;
	
	if(obj.contains("type")){
		QString type = obj["type"].toString().trimmed();
		if(type != ""){
			filters << "(e.type = :type)";
			filter_values[":type"] = type;
		}
	}
	
	if(obj.contains("search")){
		QString search = obj["search"].toString().trimmed();
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
	QSqlDatabase db = *(pWebSocketServer->database());
	
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
			jsonData["count"] = record.value("cnt").toInt();
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

	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["data"] = publiceventslist;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
