#include <cmd_handler_quest_pass.h>
#include <runtasks.h>
#include <log.h>
#include <utils.h>
#include <memory_cache_serverinfo.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdHandlerQuestPass::CmdHandlerQuestPass(){
    TAG = "CmdQuestPassHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
    m_vInputs.push_back(CmdInputDef("answer").string_().required().description("Answer"));

}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestPass::cmd(){
    return "quest_pass";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerQuestPass::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerQuestPass::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestPass::description(){
	return "Update the quest info";
}

// ---------------------------------------------------------------------

void CmdHandlerQuestPass::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    IMemoryCache *pMemoryCache = pRequest->server()->findMemoryCache("serverinfo");
    if(pMemoryCache == NULL){
        pRequest->sendMessageError(cmd(), Errors::InternalServerError());
        return;
    }
    MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);

    QSqlDatabase db = *(pRequest->server()->database());

    IUserToken *pUserToken = pRequest->userToken();
	int nUserID = 0;
    QString sNick = "";
	if(pUserToken != NULL) {
		nUserID = pUserToken->userid();
        sNick = pUserToken->nick();
	}

    int nQuestID = jsonRequest["questid"].toInt();
    QString sUserAnswer = jsonRequest["answer"].toString().trimmed();

    QString sState = "";
    QString sQuestAnswer = "";
    QString sQuestName = "";
    int nGameID = 0;
	{
		QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
		
		if (query.next()) {
			QSqlRecord record = query.record();
            sState = record.value("state").toString();
            sQuestAnswer = record.value("answer").toString().trimmed();
            sQuestName = record.value("name").toString().trimmed();
            nGameID = record.value("gameid").toInt();
		}else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
			return;
		}
	}

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE id = :gameid AND (NOW() < date_stop OR NOW() > date_restart)");
        query.bindValue(":gameid", nGameID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if (!query.next()) {
            pRequest->sendMessageError(cmd(), Error(403, "Game ended. Please wait date of restart."));
            return;
        }
    }

    // check passed quest
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) as cnt FROM users_quests WHERE questid = :questid AND userid = :userid");
        query.bindValue(":questid", nQuestID);
        query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            if(record.value("cnt").toInt() > 0){
                pRequest->sendMessageError(cmd(), Error(404, "Quest already passed"));
                return;
            }
        }
    }

    // check answer
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) as cnt FROM users_quests_answers WHERE user_answer = :user_asnwer AND questid = :questid AND userid = :userid");
        query.bindValue(":user_answer", sUserAnswer);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            if(record.value("cnt").toInt() > 0){
                pRequest->sendMessageError(cmd(), Error(404, "Your already try this answer."));
                return;
            }
        }
    }

    bool bPassed = sQuestAnswer.toUpper() == sUserAnswer.toUpper();
    QString sPassed = bPassed ? "Yes" : "No";
    int nLevenshtein = UtilsLevenshtein::distance(sUserAnswer.toUpper(), sQuestAnswer.toUpper());


    // insert to user tries
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO users_quests_answers(userid, questid, user_answer, quest_answer, passed, levenshtein, dt) "
                      "VALUES(:userid, :questid, :user_answer, :quest_answer, :passed, :levenshtein, NOW())");
        query.bindValue(":userid", nUserID);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":user_answer", sUserAnswer);
        query.bindValue(":quest_answer", sQuestAnswer);
        query.bindValue(":passed", sPassed);
        query.bindValue(":levenshtein", nLevenshtein);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        pMemoryCacheServerInfo->incrementQuestsAttempt();
    }

    if(!bPassed){
        pRequest->sendMessageError(cmd(), Error(403, "Answer incorrect. Levenshtein distance: " + QString::number(nLevenshtein)));
        return;
    }

    // insert to user passed quests
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO users_quests(userid, questid, dt_passed) "
                      "VALUES(:userid, :questid, NOW())");
        query.bindValue(":userid", nUserID);
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }
    pMemoryCacheServerInfo->incrementQuestsCompleted();


    RunTasks::AddPublicEvents(pRequest->server(), "quests", "User #" + QString::number(nUserID) + "  " + sNick
                              + " passed quest #" + QString::number(nQuestID) + " " + sQuestName);

    RunTasks::UpdateUserRating(pRequest->server(), nUserID);
    RunTasks::UpdateQuestSolved(pRequest->server(), nQuestID);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
