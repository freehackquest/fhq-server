#include <cmd_scoreboard_handler.h>
#include <QJsonArray>
#include <memory_cache_scoreboard.h>

CmdScoreboardHandler::CmdScoreboardHandler(){
	// validation and description input fields
	m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
	m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

std::string CmdScoreboardHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdScoreboardHandler::inputs(){
	return m_vInputs;
};

QString CmdScoreboardHandler::description(){
	return "Method return scoreboard";
}

QStringList CmdScoreboardHandler::errors(){
	QStringList	list;
	return list;
}

void CmdScoreboardHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));

	int nPage = obj["page"].toInt();
	jsonData["page"] = nPage;
	
	int nOnPage = obj["onpage"].toInt();
	if(nOnPage > 50){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::OnPageCouldNotBeMoreThen50());
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

	
	IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("scoreboard");
	if(pMemoryCache == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
		return;
	}

	MemoryCacheScoreboard *pMemoryCacheScoreboard = dynamic_cast<MemoryCacheScoreboard*>(pMemoryCache);
	pMemoryCacheScoreboard->loadSync();

	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["count"] = pMemoryCacheScoreboard->count();
	jsonData["data"] = pMemoryCacheScoreboard->toJsonArray();
	pWebSocketServer->sendMessage(pClient, jsonData);
}
