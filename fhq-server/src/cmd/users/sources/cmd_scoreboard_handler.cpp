#include <cmd_scoreboard_handler.h>
#include <QJsonArray>
#include <employ_scoreboard.h>

CmdScoreboardHandler::CmdScoreboardHandler()
	: CmdHandlerBase("scoreboard", "Method return scoreboard"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

	// validation and description input fields
	m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
	m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

// ---------------------------------------------------------------------

void CmdScoreboardHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

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

    EmployScoreboard *pScoreboard = findEmploy<EmployScoreboard>();
    pScoreboard->loadSync();

    jsonResponse["count"] = pScoreboard->count();
    jsonResponse["data"] = pScoreboard->toJson();

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
