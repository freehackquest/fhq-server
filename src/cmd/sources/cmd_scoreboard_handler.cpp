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

void CmdScoreboardHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    int nPage = jsonRequest["page"].toInt();
    jsonResponse["page"] = nPage;
	
    int nOnPage = jsonRequest["onpage"].toInt();
	if(nOnPage > 50){
        pRequest->sendMessageError(cmd(), Errors::OnPageCouldNotBeMoreThen50());
	}
    jsonResponse["onpage"] = nOnPage;

	QStringList filters;
	QMap<QString,QString> filter_values;
	
    if(jsonRequest.contains("user")){
        QString user = jsonRequest["user"].toString().trimmed();
		filters << "(u.nick like :nick)";
		filter_values[":nick"] = "%" + user + "%";
	}
	
	filters << "(rating > 0)";

	QString where = filters.join(" AND "); 
	if(where.length() > 0){
		where = "WHERE " + where;
	}

	
    IMemoryCache *pMemoryCache = pRequest->server()->findMemoryCache("scoreboard");
	if(pMemoryCache == NULL){
        pRequest->sendMessageError(cmd(), Errors::InternalServerError());
		return;
	}

	MemoryCacheScoreboard *pMemoryCacheScoreboard = dynamic_cast<MemoryCacheScoreboard*>(pMemoryCache);
	pMemoryCacheScoreboard->loadSync();

    jsonResponse["count"] = pMemoryCacheScoreboard->count();
    jsonResponse["data"] = pMemoryCacheScoreboard->toJsonArray();

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
