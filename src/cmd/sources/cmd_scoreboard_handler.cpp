#include <cmd_scoreboard_handler.h>
#include <QJsonArray>
#include <memory_cache_scoreboard.h>

CmdScoreboardHandler::CmdScoreboardHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

	// validation and description input fields
	m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
	m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

// ---------------------------------------------------------------------

std::string CmdScoreboardHandler::cmd(){
	return "scoreboard";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdScoreboardHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdScoreboardHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdScoreboardHandler::description(){
	return "Method return scoreboard";
}

// ---------------------------------------------------------------------

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
