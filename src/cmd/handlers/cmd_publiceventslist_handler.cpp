#include "../headers/cmd_publiceventslist_handler.h"
#include <QJsonArray>

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

QString CmdPublicEventsListHandler::short_description(){
	return "Return list of public events (news)";
}

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

void CmdPublicEventsListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

	int nPage = 0;
	if(obj.contains("page")){
		QJsonValueRef vPage = obj["page"];
		if(!vPage.isDouble()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Errors::PageMustBeInteger());
			return;
		}
		nPage = vPage.toInt();
	}
	jsonData["page"] = nPage;
	
	int nOnPage = 10;
	if(obj.contains("onpage")){
		QJsonValueRef vOnPage = obj["onpage"];
		if(!vOnPage.isDouble()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Errors::OnPageMustBeInteger());
			return;
		}
		nOnPage = vOnPage.toInt();
	}
	if(nOnPage > 50){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::OnPageCouldNotBeMoreThen50());
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
	jsonData["data"] = publiceventslist;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
