#include <cmd_handler_quests_proposal_list.h>
#include <QJsonArray>
#include <SmtpMime>
// #include <memory_cache_serverinfo.h>
#include <employ_settings.h>
#include <employ_database.h>

CmdHandlerQuestsProposalList::CmdHandlerQuestsProposalList(){
    TAG = "CmdHandlerQuestsProposalList";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("onpage").integer_().optional().description("On page"));
    m_vInputs.push_back(CmdInputDef("page").integer_().optional().description("page"));
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestsProposalList::cmd(){
        return "quests_proposal_list";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerQuestsProposalList::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerQuestsProposalList::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestsProposalList::description(){
        return "Quests proposal list";
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsProposalList::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

	QStringList filters;
	QMap<QString,QString> filter_values;

    int nPage = 0;
    int nOnPage = 5;
    int nCount = 0;

    if(jsonRequest.contains("page")){
        nPage = jsonRequest["page"].toInt();
    }

    if(jsonRequest.contains("onpage")){
        nOnPage = jsonRequest["onpage"].toInt();
    }

	;
    QSqlDatabase db = *(pDatabase->database());
	QString where = filters.join(" AND "); 
	if(where.length() > 0){
		where = "WHERE " + where;
	}

    // calculate count
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM quests_proposal " + where);
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if(query.next()) {
            QSqlRecord record = query.record();
            nCount = record.value("cnt").toInt();
        }
    }
        
	// quests_proposal
    QJsonArray quests_proposal;
    {
        QSqlQuery query(db);
        query.prepare("SELECT qp.*, u.nick, u.email, g.title as game_title  FROM quests_proposal qp "
			" LEFT JOIN users u ON u.id = qp.userid "
			" LEFT JOIN games g ON g.id = qp.gameid "
			+ where + " ORDER BY created DESC LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage));
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();

            int nID = record.value("id").toInt();
            int nUserID = record.value("userid").toInt();
            QString sEmail = record.value("email").toString();
            QString sNick = record.value("nick").toString().toHtmlEscaped();
            int nGameID = record.value("gameid").toInt();
            QString sGameTitle = record.value("game_title").toString();
            int nConfirmed = record.value("confirmed").toInt();
            QString sName = record.value("name").toString().toHtmlEscaped();
            int nScore = record.value("score").toInt();
            QString sCreated = record.value("created").toString();
            QString sSubject = record.value("subject").toString().toHtmlEscaped();
            
            QJsonObject quest_proposal;
            quest_proposal["id"] = nID;
            quest_proposal["userid"] = nUserID;
            quest_proposal["nick"] = sNick;
            quest_proposal["email"] = sEmail;
            quest_proposal["gameid"] = nGameID;
            quest_proposal["game_title"] = sGameTitle;
            quest_proposal["name"] = sName;
            quest_proposal["score"] = nScore;
            quest_proposal["created"] = sCreated;
            quest_proposal["subject"] = sSubject;
            quest_proposal["confirmed"] = nConfirmed;
            
            quests_proposal.push_back(quest_proposal);
        }
    }

    jsonResponse["data"] = quests_proposal;
    jsonResponse["onpage"] = nOnPage;
    jsonResponse["page"] = nPage;
    jsonResponse["count"] = nCount;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);

}
