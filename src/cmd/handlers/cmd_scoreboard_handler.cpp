#include "../headers/cmd_scoreboard_handler.h"
#include <QJsonArray>
#include "../../cache/headers/memory_cache_scoreboard.h"

CmdScoreboardHandler::CmdScoreboardHandler(){
	// validation and description input fields
	m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
	m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

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

const QVector<CmdInputDef> &CmdScoreboardHandler::inputs(){
	return m_vInputs;
};

QString CmdScoreboardHandler::description(){
	return "Method return scoreboard";
}

QStringList CmdScoreboardHandler::errors(){
	QStringList	list;
	return list;
}

void CmdScoreboardHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

	int nPage = obj["page"].toInt();
	jsonData["page"] = nPage;
	
	int nOnPage = obj["onpage"].toInt();
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
	
	IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("scoreboard");
	if(pMemoryCache == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::InternalServerError());
		return;
	}

	MemoryCacheScoreboard *pMemoryCacheScoreboard = dynamic_cast<MemoryCacheScoreboard*>(pMemoryCache);
	pMemoryCacheScoreboard->loadSync();

	jsonData["result"] = QJsonValue("DONE");
	jsonData["count"] = pMemoryCacheScoreboard->count();
	jsonData["data"] = pMemoryCacheScoreboard->toJsonArray();
	pWebSocketServer->sendMessage(pClient, jsonData);
}
