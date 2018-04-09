#include <cmd_handlers_quests.h>
#include <runtasks.h>
#include <log.h>
#include <utils.h>
#include <employ_settings.h>
#include <QCryptographicHash>
#include <employ_database.h>
#include <memory_cache_serverinfo.h>

// *******************************************
// *************** Quest List ****************
// *******************************************

CmdHandlerQuests::CmdHandlerQuests()
    : CmdHandlerBase("quests", "Method will be returned quest list"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("subject").string_().optional().description("Filter by subject"));
}

// ---------------------------------------------------------------------

void CmdHandlerQuests::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    int nUserID = 0;
    IUserToken *pUserToken = pRequest->userToken();
    if(pUserToken != NULL){
        nUserID = pUserToken->userid();
    }

    std::vector<std::string> vWhereQuery;
    std::vector<std::pair<std::string, std::string> > vWhereValues;

    // get quest id
    if(jsonRequest["subject"].is_string()){
        std::string sSubject = jsonRequest.at("subject");
        vWhereQuery.push_back("(q.subject = :subject)");
        vWhereValues.push_back(std::pair<std::string, std::string>(":subject",sSubject));
    }

    if((pUserToken != NULL && !pUserToken->isAdmin()) || (pUserToken == NULL)){
        vWhereQuery.push_back("(q.state = :state)");
        vWhereValues.push_back(std::pair<std::string, std::string>(":state","open"));
    }

    std::string sWhere = "";
    for(unsigned int i = 0; i < vWhereQuery.size(); i++){
        sWhere += sWhere.length() > 0 ? " AND " : "";
        sWhere += vWhereQuery[i];

    }
    sWhere = sWhere.length() > 0 ? " WHERE " + sWhere : "";
    std::cout << "sWhere: " << sWhere << std::endl;

    auto jsonQuests = nlohmann::json::array();

    QSqlDatabase db = *(pDatabase->database());
	QSqlQuery query(db);

    query.prepare(""
                  "SELECT "
                  "     q.idquest, "
                  "     q.name, "
                  "     q.score, "
                  "     q.subject, "
                  "     q.gameid, "
                  "     q.state, "
                  "     q.count_user_solved, "
                  "     uq.dt_passed "
                  "FROM "
                  "   quest q "
                  "LEFT JOIN"
                  "    users_quests uq ON uq.questid = q.idquest AND uq.userid = :userid "
                  + QString(sWhere.c_str())
                  + "ORDER BY "
                  "    q.subject, q.score ASC, q.score; ");

    query.bindValue(":userid", nUserID);

    for(unsigned int i = 0; i < vWhereValues.size(); i++){
        std::pair<std::string, std::string> p = vWhereValues[i];
        query.bindValue(QString(p.first.c_str()), QString(p.second.c_str()));
    }

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
	while (query.next()) {
		QSqlRecord record = query.record();
        nlohmann::json jsonQuest;
        jsonQuest["questid"] = record.value("idquest").toInt();
        jsonQuest["score"] = record.value("score").toInt();
        jsonQuest["name"] = record.value("name").toString().toStdString();
        jsonQuest["gameid"] = record.value("gameid").toInt();
        jsonQuest["subject"] = record.value("subject").toString().toStdString();
        jsonQuest["dt_passed"] = record.value("dt_passed").toString().toStdString();
        jsonQuest["solved"] = record.value("count_user_solved").toInt();
        jsonQuest["state"] = record.value("state").toString().toStdString();

        std::string status = record.value("dt_passed").toString().toStdString();
        jsonQuest["status"] = (status == "" ? "open" : "completed");

        jsonQuests.push_back(jsonQuest);
	}

    jsonResponse["data"] = jsonQuests;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// *************** Quest Info ****************
// *******************************************


CmdHandlerQuest::CmdHandlerQuest()
    : CmdHandlerBase("quest", "Update the quest info"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
}

// ---------------------------------------------------------------------

void CmdHandlerQuest::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString sBaseGamesURL = pSettings->getSettString("server_folder_games_url");

    IUserToken *pUserToken = pRequest->userToken();
    bool bAdmin = false;
    int nUserID = 0;
    if(pUserToken != NULL) {
        bAdmin = pUserToken->isAdmin();
        nUserID = pUserToken->userid();
    }

    int nQuestID = jsonRequest["questid"].toInt();

    {
        QSqlQuery query(db);
        query.prepare("SELECT "
            "	q.idquest, "
            "	q.gameid, "
            "	q.name, "
            "	q.text, "
            "	q.answer_format, "
            "	q.score, "
            "	q.subject, "
            "	q.copyright, "
            "	q.state, "
            "	q.author, "
            "	q.count_user_solved, "
            "	q.answer, "
            "	q.description_state, "
            " 	users_quests.dt_passed as dt_passed2"
            " FROM "
            "	quest q "
            " LEFT JOIN "
            "	users_quests ON users_quests.questid = q.idquest AND users_quests.userid = :userid"
            " WHERE "
            "	q.idquest = :questid"
        );
        query.bindValue(":userid", nUserID);
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            QJsonObject quest;
            quest["id"] = record.value("idquest").toInt();
            int nGameID = record.value("gameid").toInt();
            QString sState = record.value("state").toString();
            QString sPassed = record.value("dt_passed2").toString();

            if(sState == "open" || bAdmin){
                quest["gameid"] = nGameID;
                quest["name"] = record.value("name").toString();
                quest["text"] = record.value("text").toString();
                quest["answer_format"] = record.value("answer_format").toString();
                quest["score"] = record.value("score").toInt();
                quest["subject"] = record.value("subject").toString();
                quest["copyright"] = record.value("copyright").toString();
                quest["state"] = record.value("state").toString();
                quest["author"] = record.value("author").toString();
                quest["count_user_solved"] = record.value("count_user_solved").toString();
            }

            // user completed quest
            quest["completed"] = !record.isNull("dt_passed2");
            quest["dt_passed"] = sPassed;

            if(bAdmin){
                quest["answer"] = record.value("answer").toString();
                quest["description_state"] = record.value("description_state").toString();
            }
            jsonResponse["quest"] = quest;

            // game info
            {
                QJsonObject game;
                QSqlQuery query_game(db);
                query_game.prepare("SELECT * FROM games WHERE id = :id");
                query_game.bindValue(":id", nGameID);
                if(!query_game.exec()){
                    pRequest->sendMessageError(cmd(), Error(500, query_game.lastError().text()));
                    return;
                }
                if (query_game.next()) {
                    QSqlRecord record_game = query_game.record();
                    int nGameID = record_game.value("id").toInt();
                    game["id"] = nGameID;
                    game["title"] = record_game.value("title").toString();
                    game["logo"] = sBaseGamesURL + QString::number(nGameID) + ".png";
                }else{
                    pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
                    return;
                }
                jsonResponse["game"] = game;
            }


            // files
            {
                QJsonArray files;
                QSqlQuery query_files(db);
                query_files.prepare("SELECT * FROM quests_files WHERE questid = :questid");
                query_files.bindValue(":questid", nQuestID);
                if(!query_files.exec()){
                    pRequest->sendMessageError(cmd(), Error(500, query_files.lastError().text()));
                    return;
                }
                while (query_files.next()) {
                    QSqlRecord record_game = query_files.record();
                    QJsonObject fileinfo;
                    fileinfo["id"] = record_game.value("id").toInt();
                    fileinfo["uuid"] = record_game.value("uuid").toInt();
                    fileinfo["filename"] = record_game.value("filename").toString();
                    fileinfo["size"] = record_game.value("size").toString();
                    fileinfo["dt"] = record_game.value("dt").toString();
                    fileinfo["filepath"] = record_game.value("filepath").toString();
                    files.append(fileinfo);
                }
                jsonResponse["files"] = files;
            }

            // hints
            {
                QJsonArray hints;
                QSqlQuery query_hints(db);
                query_hints.prepare("SELECT * FROM quests_hints WHERE questid = :questid");
                query_hints.bindValue(":questid", nQuestID);
                if(!query_hints.exec()){
                    pRequest->sendMessageError(cmd(), Error(500, query_hints.lastError().text()));
                    return;
                }
                while (query_hints.next()) {
                    QSqlRecord record_game = query_hints.record();
                    QJsonObject hint;
                    hint["id"] = record_game.value("id").toInt();
                    hint["text"] = record_game.value("text").toString();
                    hints.append(hint);
                }
                jsonResponse["hints"] = hints;
            }

        }else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// *************** Quest Pass ****************
// *******************************************

CmdHandlerQuestPass::CmdHandlerQuestPass()
: CmdHandlerBase("quest_pass", "Quest pass"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
    m_vInputs.push_back(CmdInputDef("answer").string_().required().description("Answer"));

}

// ---------------------------------------------------------------------

void CmdHandlerQuestPass::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    IMemoryCache *pMemoryCache = pRequest->server()->findMemoryCache("serverinfo");
    if(pMemoryCache == NULL){
        pRequest->sendMessageError(cmd(), Errors::InternalServerError());
        return;
    }
    MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);

    QSqlDatabase db = *(pDatabase->database());

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
