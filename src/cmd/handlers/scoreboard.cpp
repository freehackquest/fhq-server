#include "../headers/scoreboard.h"
#include <QJsonArray>

QString CmdScoreboardHandler::cmd(){
	return "scoreboard";
}

bool CmdScoreboardHandler::accessUnauthorized(){
	return true;
}

bool CmdScoreboardHandler::accessUser(){
	return true;
}

bool CmdScoreboardHandler::accessTester(){
	return true;
}

bool CmdScoreboardHandler::accessAdmin(){
	return true;
}

QString CmdScoreboardHandler::short_description(){
	return "some short description";
}

QString CmdScoreboardHandler::description(){
	return "some description";
}

QStringList CmdScoreboardHandler::errors(){
	QStringList	list;
	return list;
}

void CmdScoreboardHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){

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
	
	int nOnPage = 15;
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
	}
	jsonData["onpage"] = nOnPage;

	QStringList filters;
	QMap<QString,QString> filter_values;
	
	if(obj.contains("user")){
		QString user = obj["user"].toString().trimmed();
		filters << "(u.nick like :nick)";
		filter_values[":nick"] = "%" + user + "%";
	}
	
	filters << "(rating > 0)";

	QString where = filters.join(" AND "); 
	if(where.length() > 0){
		where = "WHERE " + where;
	}
	
	// count quests
	QSqlDatabase db = *(pWebSocketServer->database());
	/*{
		QSqlQuery query(db);
		query.prepare("SELECT count(*) as cnt FROM " + table + " ta "
			" INNER JOIN users u ON u.id = ta.iduser"
			" INNER JOIN quest q ON q.idquest = ta.idquest"
			" INNER JOIN games g ON g.id = ta.gameid"
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
	}*/

	// data
	// TODO add place
	QJsonArray users;
	{
		QSqlQuery query(db);
		query.prepare("SELECT "
			" u.id, "
			" u.nick, "
			" u.logo, "
			" u.rating "
			" FROM users u "
			" " + where + 
			" ORDER BY rating DESC "
			// " LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
		);
		foreach(QString key, filter_values.keys() ){
			query.bindValue(key, filter_values.value(key));
		}
		
		query.exec();
		while (query.next()) {
			QSqlRecord record = query.record();
			QJsonObject user;
			user["userid"] = record.value("id").toInt();
			user["nick"] = record.value("nick").toString(); // TODO htmlspecialchars
			user["logo"] = record.value("logo").toString(); // TODO htmlspecialchars
			user["rating"] = record.value("rating").toInt();
			users.push_back(user);
		}
	}

	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = users;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
