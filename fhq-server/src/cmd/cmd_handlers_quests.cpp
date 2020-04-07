#include <cmd_handlers_quests.h>
#include <runtasks.h>
#include <wsjcpp_levenshtein.h>
#include <fallen.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <employ_notify.h>
#include <validators.h>
#include <wsjcpp_hashes.h>

// *******************************************
// *************** Quest List ****************
// *******************************************

CmdHandlerQuests::CmdHandlerQuests()
    : CmdHandlerBase("quests", "Method will be returned quest list") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    optionalStringParam("subject", "Filter by subject")
        .addValidator(new ValidatorQuestSubject());
    optionalIntegerParam("gameid", "Filter by local gameid");
    optionalStringParam("filter", "Filter by some text");
}

// ---------------------------------------------------------------------

void CmdHandlerQuests::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    int nUserID = 0;
    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    if (pUserSession != nullptr) {
        nUserID = pUserSession->userid();
    }

    std::vector<std::string> vWhereQuery;
    std::vector<std::pair<std::string, std::string> > vWhereValues;

    std::string sFilter = pRequest->getInputString("filter", "");
    if (sFilter != "") {
        sFilter = "%" + sFilter + "%";
        vWhereQuery.push_back("(q.name LIKE :filter OR q.copyright LIKE :copyright OR q.text LIKE :text)");
        vWhereValues.push_back(std::pair<std::string, std::string>(":filter", sFilter));
        vWhereValues.push_back(std::pair<std::string, std::string>(":copyright", sFilter));
        vWhereValues.push_back(std::pair<std::string, std::string>(":text", sFilter));
    }

    // get quest id
    std::string sSubject = pRequest->getInputString("subject", "");
    if (sSubject != "") {
        vWhereQuery.push_back("(q.subject = :subject)");
        vWhereValues.push_back(std::pair<std::string, std::string>(":subject",sSubject));
    }

    // TODO change to uuid
    int nGameId =pRequest->getInputInteger("gameid", 0);
    if (nGameId != 0) {
        vWhereQuery.push_back("(q.gameid = :gameid)");
        vWhereValues.push_back(std::pair<std::string, std::string>(":gameid", std::to_string(nGameId)));
    }

    if (!pRequest->isAdmin()) {
        vWhereQuery.push_back("(q.state = :state)");
        vWhereValues.push_back(std::pair<std::string, std::string>(":state","open"));
    }

    std::string sWhere = "";
    for (unsigned int i = 0; i < vWhereQuery.size(); i++) {
        sWhere += sWhere.length() > 0 ? " AND " : "";
        sWhere += vWhereQuery[i];

    }
    sWhere = sWhere.length() > 0 ? " WHERE " + sWhere : "";
    // std::cout << "sWhere: " << sWhere << std::endl;

    nlohmann::json jsonQuests = nlohmann::json::array();

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

    for (unsigned int i = 0; i < vWhereValues.size(); i++) {
        std::pair<std::string, std::string> p = vWhereValues[i];
        query.bindValue(QString(p.first.c_str()), QString(p.second.c_str()));
    }

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
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
    : CmdHandlerBase("quest", "Update the quest info") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
}

// ---------------------------------------------------------------------

void CmdHandlerQuest::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

    QString sBaseGamesURL = QString::fromStdString(pGlobalSettings->get("server_folder_games_url").getStringValue());

    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    bool bAdmin = false;
    int nUserID = 0;
    if (pUserSession != nullptr) {
        bAdmin = pUserSession->isAdmin();
        nUserID = pUserSession->userid();
    }

    int nQuestID = pRequest->getInputInteger("questid", 0);

    {
        QSqlQuery query(db);
        query.prepare("SELECT "
            "    q.idquest, "
            "    q.gameid, "
            "    q.uuid, "
            "    q.name, "
            "    q.text, "
            "    q.answer_format, "
            "    q.score, "
            "    q.subject, "
            "    q.copyright, "
            "    q.state, "
            "    q.author, "
            "    q.count_user_solved, "
            "    q.answer, "
            "    q.description_state, "
            "     users_quests.dt_passed as dt_passed2"
            " FROM "
            "    quest q "
            " LEFT JOIN "
            "    users_quests ON users_quests.questid = q.idquest AND users_quests.userid = :userid"
            " WHERE "
            "    q.idquest = :questid"
        );
        query.bindValue(":userid", nUserID);
        query.bindValue(":questid", nQuestID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonQuest;
            jsonQuest["id"] = record.value("idquest").toInt();
            jsonQuest["uuid"] = record.value("uuid").toString().toStdString();
            int nGameID = record.value("gameid").toInt();
            QString sState = record.value("state").toString();
            QString sPassed = record.value("dt_passed2").toString();

            if (sState == "open" || bAdmin) {
                jsonQuest["gameid"] = nGameID;
                jsonQuest["name"] = record.value("name").toString().toStdString();
                jsonQuest["text"] = record.value("text").toString().toStdString();
                jsonQuest["answer_format"] = record.value("answer_format").toString().toStdString();
                jsonQuest["score"] = record.value("score").toInt();
                jsonQuest["subject"] = record.value("subject").toString().toStdString();
                jsonQuest["copyright"] = record.value("copyright").toString().toStdString();
                jsonQuest["state"] = record.value("state").toString().toStdString();
                jsonQuest["author"] = record.value("author").toString().toStdString();
                jsonQuest["count_user_solved"] = record.value("count_user_solved").toString().toStdString();
            }

            // user completed quest
            jsonQuest["completed"] = !record.isNull("dt_passed2");
            jsonQuest["dt_passed"] = sPassed.toStdString();

            if (bAdmin) {
                jsonQuest["answer"] = record.value("answer").toString().toStdString();
                jsonQuest["description_state"] = record.value("description_state").toString().toStdString();
            }
            jsonResponse["quest"] = jsonQuest;

            // game info
            {
                nlohmann::json jsonGame;
                QSqlQuery query_game(db);
                query_game.prepare("SELECT * FROM games WHERE id = :id");
                query_game.bindValue(":id", nGameID);
                if (!query_game.exec()) {
                    pRequest->sendMessageError(cmd(), WsjcppError(500, query_game.lastError().text().toStdString()));
                    return;
                }
                if (query_game.next()) {
                    QSqlRecord record_game = query_game.record();
                    int nGameID = record_game.value("id").toInt();
                    jsonGame["id"] = nGameID;
                    jsonGame["title"] = record_game.value("title").toString().toStdString();
                    jsonGame["logo"] = QString(sBaseGamesURL + QString::number(nGameID) + ".png").toStdString();
                } else {
                    pRequest->sendMessageError(cmd(), WsjcppError(404, "Game not found"));
                    return;
                }
                jsonResponse["game"] = jsonGame;
            }


            // files
            {
                nlohmann::json jsonFiles = nlohmann::json::array();
                QSqlQuery query_files(db);
                query_files.prepare("SELECT * FROM quests_files WHERE questid = :questid");
                query_files.bindValue(":questid", nQuestID);
                if (!query_files.exec()) {
                    pRequest->sendMessageError(cmd(), WsjcppError(500, query_files.lastError().text().toStdString()));
                    return;
                }
                while (query_files.next()) {
                    QSqlRecord record_game = query_files.record();
                    nlohmann::json jsonFileInfo;
                    jsonFileInfo["id"] = record_game.value("id").toInt();
                    jsonFileInfo["uuid"] = record_game.value("uuid").toInt();
                    jsonFileInfo["filename"] = record_game.value("filename").toString().toStdString();
                    jsonFileInfo["size"] = record_game.value("size").toString().toStdString();
                    jsonFileInfo["dt"] = record_game.value("dt").toString().toStdString();
                    jsonFileInfo["filepath"] = record_game.value("filepath").toString().toStdString();
                    jsonFiles.push_back(jsonFileInfo);
                }
                jsonResponse["files"] = jsonFiles;
            }

            // TODO: deprecated
            // hints 
            {
                nlohmann::json jsonHints = nlohmann::json::array();

                QSqlQuery query_hints(db);
                query_hints.prepare("SELECT * FROM quests_hints WHERE questid = :questid");
                query_hints.bindValue(":questid", nQuestID);
                if (!query_hints.exec()) {
                    pRequest->sendMessageError(cmd(), WsjcppError(500, query_hints.lastError().text().toStdString()));
                    return;
                }
                while (query_hints.next()) {
                    QSqlRecord record_game = query_hints.record();
                    nlohmann::json jsonHint;
                    jsonHint["id"] = record_game.value("id").toInt();
                    jsonHint["text"] = record_game.value("text").toString().toStdString();
                    jsonHints.push_back(jsonHint);
                }
                jsonResponse["hints"] = jsonHints;
            }
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Quest not found"));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// *************** Quest Pass ****************
// *******************************************

CmdHandlerQuestPass::CmdHandlerQuestPass()
: CmdHandlerBase("quest_pass", "Quest pass") {

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
    requireStringParam("answer", "Answer");

}

// ---------------------------------------------------------------------

void CmdHandlerQuestPass::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findWsjcppEmploy<EmployServerInfo>();

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    int nUserID = 0;
    QString sNick = "";
    if (pUserSession != NULL) {
        nUserID = pUserSession->userid();
        sNick = pUserSession->nick();
    }

    int nQuestID = pRequest->getInputInteger("questid", 0);
    std::string sUserAnswer = pRequest->getInputString("answer", "");
    WsjcppCore::trim(sUserAnswer);

    QString sState = "";
    QString sQuestAnswer = "";
    QString sQuestName = "";
    int nGameID = 0;
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            sState = record.value("state").toString();
            sQuestAnswer = record.value("answer").toString().trimmed();
            sQuestName = record.value("name").toString().trimmed();
            nGameID = record.value("gameid").toInt();
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Quest not found"));
            return;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE id = :gameid AND (NOW() < date_stop OR NOW() > date_restart)");
        query.bindValue(":gameid", nGameID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }

        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(403, "Game ended. Please wait date of restart."));
            return;
        }
    }

    // check passed quest
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) as cnt FROM users_quests WHERE questid = :questid AND userid = :userid");
        query.bindValue(":questid", nQuestID);
        query.bindValue(":userid", nUserID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            if (record.value("cnt").toInt() > 0) {
                pRequest->sendMessageError(cmd(), WsjcppError(404, "Quest already passed"));
                return;
            }
        }
    }

    // check answer
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) as cnt FROM users_quests_answers WHERE user_answer = :user_asnwer AND questid = :questid AND userid = :userid");
        query.bindValue(":user_answer", QString::fromStdString(sUserAnswer));
        query.bindValue(":questid", nQuestID);
        query.bindValue(":userid", nUserID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            if (record.value("cnt").toInt() > 0) {
                pRequest->sendMessageError(cmd(), WsjcppError(404, "Your already try this answer."));
                return;
            }
        }
    }

    bool bPassed = sQuestAnswer.toUpper() == QString::fromStdString(sUserAnswer).toUpper();
    QString sPassed = bPassed ? "Yes" : "No";
    int nLevenshtein = WsjcppLevenshtein::distance(QString::fromStdString(sUserAnswer).toUpper().toStdString(), sQuestAnswer.toUpper().toStdString());


    // insert to user tries
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO users_quests_answers(userid, questid, user_answer, quest_answer, passed, levenshtein, dt) "
                      "VALUES(:userid, :questid, :user_answer, :quest_answer, :passed, :levenshtein, NOW())");
        query.bindValue(":userid", nUserID);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":user_answer", QString::fromStdString(sUserAnswer));
        query.bindValue(":quest_answer", sQuestAnswer);
        query.bindValue(":passed", sPassed);
        query.bindValue(":levenshtein", nLevenshtein);

        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        pServerInfo->incrementQuestsAttempt();
    }

    if (!bPassed) {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "Answer incorrect. Levenshtein distance: " + QString::number(nLevenshtein).toStdString()));
        return;
    }

    // insert to user passed quests
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO users_quests(userid, questid, dt_passed) "
                      "VALUES(:userid, :questid, NOW())");
        query.bindValue(":userid", nUserID);
        query.bindValue(":questid", nQuestID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }
    pServerInfo->incrementQuestsCompleted();

    EmployNotify *pEmployNotify = findWsjcppEmploy<EmployNotify>();

    pEmployNotify->notifyInfo("quests", "[user#" + std::to_string(nUserID) + "](" + sNick.toStdString() + ") "
                              + " passed [quest#" + std::to_string(nQuestID) + "] (" + sQuestName.toStdString() + ")");

    RunTasks::UpdateUserRating(nUserID);
    RunTasks::UpdateQuestSolved(nQuestID);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Create ***************
// *******************************************

CmdHandlerCreateQuest::CmdHandlerCreateQuest()
    : CmdHandlerBase("createquest", "Method will be create new quest") {
    // TODO add symlinks

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireStringParam("uuid", "Global Identificator of the quest")
        .addValidator(new WsjcppValidatorUUID());

    requireIntegerParam("gameid", "Which game included this quest");
    requireStringParam("name", "Name of the quest");
    requireStringParam("text", "Description of the quest");
    requireIntegerParam("score", "How much append to user score after solve quest by them").minval(1).maxval(1000);
    // TODO validator score

    requireStringParam("author", "Author of the quest");
    requireStringParam("subject", "Subject must be one from types")
        .addValidator(new ValidatorQuestSubject());
    requireStringParam("answer", "Answer for the quest");
    requireStringParam("answer_format", "Answer format for the quest");
    requireStringParam("state", "State of the quest")
        .addValidator(new ValidatorQuestState());
    requireStringParam("description_state", "You can add some descriptions for quest state");
    optionalStringParam("copyright", "You can add some copyright information");
}

// ---------------------------------------------------------------------

void CmdHandlerCreateQuest::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findWsjcppEmploy<EmployServerInfo>();

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    std::string sUUID = pRequest->getInputString("uuid", "");
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE uuid = :uuid");
        query.bindValue(":uuid", QString::fromStdString(sUUID));
        query.exec();
        if (query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(403, "Quest with uuid [quest#" + sUUID + "] already exists"));
            return;
        }
    }

    int nGameID = pRequest->getInputInteger("gameid", 0);
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE id = :id");
        query.bindValue(":id", nGameID);
        query.exec();
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Game not found"));
            return;
        }
    }

    std::string sName = pRequest->getInputString("name", "");
    WsjcppCore::trim(sName);

    /*if (sName.length() == 0) {
        pRequest->sendMessageError(cmd(), Error(400, "Name could not be empty"));
        return;
    }*/

    std::string sText = pRequest->getInputString("text", "");
    WsjcppCore::trim(sText);
    int nScore = pRequest->getInputInteger("score", 0);
    std::string sSubject = pRequest->getInputString("subject", "");
    WsjcppCore::trim(sSubject);

    std::string sAnswer = pRequest->getInputString("answer", "");
    WsjcppCore::trim(sAnswer);
    std::string sAuthor = pRequest->getInputString("author", "");
    WsjcppCore::trim(sAuthor);
    std::string sAnswerFormat = pRequest->getInputString("answer_format", "");
    WsjcppCore::trim(sAnswerFormat);
    std::string sState = pRequest->getInputString("state", "");
    WsjcppCore::trim(sState);
    std::string sCopyright = pRequest->getInputString("copyright", "");
    WsjcppCore::trim(sCopyright);
    std::string sDescriptionState = pRequest->getInputString("description_state", "");
    WsjcppCore::trim(sDescriptionState);

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO quest("
        "        uuid,"
        "        gameid,"
        "        name,"
        "        text,"
        "        answer,"
        "        answer_upper_md5,"
        "        answer_format,"
        "        score,"
        "        author,"
        "        subject,"
        "        state,"
        "        description_state,"
        "        copyright,"
        "        count_user_solved,"
        "        date_change,"
        "        date_create"
        "    )"
        "    VALUES("
        "        :uuid,"
        "        :gameid,"
        "        :name,"
        "        :text,"
        "        :answer,"
        "        :answer_upper_md5,"
        "        :answer_format,"
        "        :score,"
        "        :author,"
        "        :subject,"
        "        :state,"
        "        :description_state,"
        "        :copyright,"
        "        :count_user_solved,"
        "        NOW(),"
        "        NOW()"
        "    )");
    query.bindValue(":uuid", QString::fromStdString(sUUID));
    query.bindValue(":gameid", nGameID);
    query.bindValue(":name", QString::fromStdString(sName));
    query.bindValue(":text", QString::fromStdString(sText));
    query.bindValue(":answer", QString::fromStdString(sAnswer));
    sAnswer = QString::fromStdString(sAnswer).toUpper().toStdString();
    std::string sAnswerUpperMd5 = WsjcppHashes::md5_calc_hex(sAnswer);
    query.bindValue(":answer_upper_md5", QString::fromStdString(sAnswerUpperMd5));
    query.bindValue(":answer_format", QString::fromStdString(sAnswerFormat));
    query.bindValue(":score", nScore);
    query.bindValue(":author", QString::fromStdString(sAuthor));
    query.bindValue(":subject", QString::fromStdString(sSubject));
    query.bindValue(":state", QString::fromStdString(sState));
    query.bindValue(":description_state", QString::fromStdString(sDescriptionState));
    query.bindValue(":copyright", QString::fromStdString(sCopyright));
    query.bindValue(":count_user_solved", 0);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    pServerInfo->incrementQuests();


    int rowid = query.lastInsertId().toInt();
    jsonResponse["questid"] = rowid;

    RunTasks::AddPublicEvents("quests", "New [quest#" + std::to_string(rowid) + "] " + sName + " (subject: " + sSubject + ")");
    RunTasks::UpdateMaxScoreGame(nGameID);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Delete ***************
// *******************************************


CmdHandlerQuestDelete::CmdHandlerQuestDelete()
    : CmdHandlerBase("quest_delete", "Method for delete quest") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestDelete::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findWsjcppEmploy<EmployServerInfo>();

    nlohmann::json jsonResponse;

    int questid = pRequest->getInputInteger("questid", 0);
    std::string sName = "";
    std::string sSubject = "";

    QSqlDatabase db = *(pDatabase->database());
    {
        // TODO: redesign EmployQuests -> findQuestById();
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", questid);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            sName = record.value("name").toString().toStdString();
            sSubject = record.value("subject").toString().toStdString();
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Quest not found"));
            return;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", questid);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    // remove from users_quests_answers
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM users_quests_answers WHERE questid = :questid");
        query.bindValue(":questid", questid);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    // remove from users_quests
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM users_quests WHERE questid = :questid");
        query.bindValue(":questid", questid);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }
    pServerInfo->decrementQuests();

    RunTasks::AddPublicEvents("quests", "Removed [quest#" + std::to_string(questid) + "] " + sName + " (subject: " + sSubject + ")");

    // TODO recalculate rating/score for users how solved this quest

    jsonResponse["subject"] = sSubject;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Proposal ***************
// *******************************************


CmdHandlerQuestProposal::CmdHandlerQuestProposal()
    : CmdHandlerBase("quest_proposal", "Add quest proposal") {
    TAG = "CmdQuestProposalHandler";

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("gameid", "Which game included this quest");
    requireStringParam("name", "Name of the quest");
    requireStringParam("text", "Description of the quest");
    requireIntegerParam("score", "How much append to user score after solve quest by them").minval(1).maxval(1000);

    requireStringParam("author", "Author of the quest");
    requireStringParam("subject", "Subject must be one from types (look types)")
        .addValidator(new ValidatorQuestSubject());
    requireStringParam("answer", "Answer for the quest");
    requireStringParam("answer_format", "Answer format for the quest");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestProposal::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

    QSqlDatabase db = *(pDatabase->database());

    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    int nUserID = 0;
    QString sUserEmail = "";
    if (pUserSession != nullptr) {
        nUserID = pUserSession->userid();
        sUserEmail = pUserSession->email();
    }

    // check the user already sended quest
    int nGameID = pRequest->getInputInteger("gameid", 0);
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE id = :id");
        query.bindValue(":id", nGameID);
        query.exec();
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Game not found"));
            return;
        }
    }

    std::string sName = pRequest->getInputString("name", "");
    WsjcppCore::trim(sName);

    if (sName.length() == 0) { // TODO to validators
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Name could not be empty"));
        return;
    }

    std::string sText = pRequest->getInputString("text", "");
    WsjcppCore::trim(sText);

    int nScore = pRequest->getInputInteger("score", 0);
    std::string sSubject = pRequest->getInputString("subject", "");
    WsjcppCore::trim(sSubject);

    std::string sAnswer = pRequest->getInputString("answer", "");
    WsjcppCore::trim(sAnswer);

    if (sAnswer.length() == 0) { // TODO to validators
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Answer could not be empty"));
        return;
    }

    std::string sAuthor = pRequest->getInputString("author", "");
    WsjcppCore::trim(sAuthor);

    if (sAuthor.length() == 0) { // TODO to validators
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Author could not be empty"));
        return;
    }

    std::string sAnswerFormat = pRequest->getInputString("answer_format", "");
    WsjcppCore::trim(sAuthor);

    if (sAnswerFormat.length() == 0) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Answer Format could not be empty"));
        return;
    }

    std::string sCopyright = pRequest->getInputString("copyright", "");
    WsjcppCore::trim(sCopyright);

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO quests_proposal("
        "        userid,"
        "        gameid,"
        "        name,"
        "        text,"
        "        answer,"
        "        answer_format,"
        "        score,"
        "        author,"
        "        subject,"
        "        copyright,"
        "        created,"
        "        updated,"
        "        confirmed"
        "    )"
        "    VALUES("
        "        :userid,"
        "        :gameid,"
        "        :name,"
        "        :text,"
        "        :answer,"
        "        :answer_format,"
        "        :score,"
        "        :author,"
        "        :subject,"
        "        :copyright,"
        "        NOW(),"
        "        NOW(),"
        "        :confirmed"
        "    )");
    query.bindValue(":userid", nUserID);
    query.bindValue(":gameid", nGameID);
    query.bindValue(":name", QString::fromStdString(sName));
    query.bindValue(":text", QString::fromStdString(sText));
    query.bindValue(":answer", QString::fromStdString(sAnswer));
    query.bindValue(":answer_format", QString::fromStdString(sAnswerFormat));
    query.bindValue(":score", nScore);
    query.bindValue(":author", QString::fromStdString(sAuthor));
    query.bindValue(":subject", QString::fromStdString(sSubject));
    query.bindValue(":copyright", QString::fromStdString(sCopyright));
    query.bindValue(":confirmed", 0);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    // pMemoryCacheServerInfo->incrementQuests();


    int nQuestProposalId = query.lastInsertId().toInt();
    jsonResponse["questid"] = nQuestProposalId;

    // TODO move to EmployMails
    std::string sMailToAdmin = pGlobalSettings->get("mail_system_message_admin_email").getStringValue();
    std::string sMessageSubject = "Quest Proposal (FreeHackQuest)";
    std::string sContext = "Quest Proposal \n"
                       "UserID: " + QString::number(nUserID).toStdString() + "\n"
                       "From: " + sUserEmail.toStdString() + "\n"
                       "QuestProposalId: #" + QString::number(nQuestProposalId).toStdString() + "\n";

    RunTasks::MailSend(sMailToAdmin, sMessageSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Statistics ***********
// *******************************************

CmdHandlerQuestStatistics::CmdHandlerQuestStatistics()
    : CmdHandlerBase("quest_statistics", "Mehtod will be return quest public statistics") {

    TAG = "CmdHandlerQuestStatistics";

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestStatistics::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int nQuestID = pRequest->getInputInteger("questid", 0);

    // check quest exists
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }

        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Quest not found"));
            return;
        }
    }


    QString sQuery = "SELECT COUNT(t0.id) AS cnt FROM users_quests_answers t0 "
    "INNER JOIN users t1 ON t1.id = t0.userid "
    "WHERE t0.questid = :questid AND t0.passed = :passed AND t1.role = :role ";

    // tries
    {
        QSqlQuery query(db);
        query.prepare(sQuery);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":passed", "No");
        query.bindValue(":role", "user");
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["tries"] = record.value("cnt").toInt();
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Quest not found"));
            return;
        }
    }

    // solved
    {
        QSqlQuery query(db);
        query.prepare(sQuery);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":passed", "Yes");
        query.bindValue(":role", "user");
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["solved"] = record.value("cnt").toInt();
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Quest not found"));
            return;
        }
    }

    // how solved this quest
    {
        QSqlQuery query(db);
        query.prepare("SELECT t0.id, t0.logo, t0.nick, t0.university FROM users t0 "
                      "INNER JOIN users_quests t1 ON t0.id = t1.userid "
                      "WHERE t0.role = :role AND t1.questid = :questid ");
        query.bindValue(":role", "user");
        query.bindValue(":questid", nQuestID);

        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        nlohmann::json jsonUsers = nlohmann::json::array();
        while (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonUser;
            jsonUser["userid"] = record.value("id").toInt();
            jsonUser["logo"] = record.value("logo").toString().toHtmlEscaped().toStdString();
            jsonUser["nick"] = record.value("nick").toString().toHtmlEscaped().toStdString();
            jsonUser["university"] = record.value("university").toString().toHtmlEscaped().toStdString();
            jsonUsers.push_back(jsonUser);
        }
        jsonResponse["users"] = jsonUsers;
    }

    // TODO check this
    // jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Update ***********
// *******************************************


CmdHandlerQuestUpdate::CmdHandlerQuestUpdate()
    : CmdHandlerBase("quest_update", "Update the quest info") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
    optionalStringParam("name", "Name of the quest");
    optionalIntegerParam("gameid", "Which game included this quest");
    optionalStringParam("text", "Description of the quest"); // addModificator (for example ModificatorStringTrim)
        
    optionalIntegerParam("score", "How much append to user score after solve quest by them").minval(1).maxval(1000); // TODO validator

    optionalStringParam("subject", "Subject must be one from types")
        .addValidator(new ValidatorQuestSubject());
    optionalStringParam("author", "Author of the quest");
    optionalStringParam("answer", "Answer for the quest");
    optionalStringParam("answer_format", "Answer format for the quest");
    optionalStringParam("state", "State of the quest")
        .addValidator(new ValidatorQuestState());
    optionalStringParam("description_state", "You can update some descriptions for quest state");
    optionalStringParam("copyright", "You can update copyright");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestUpdate::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int nQuestID = pRequest->getInputInteger("questid", 0);
    std::string sNamePrev = "";
    std::string sTextPrev = "";
    int nScorePrev = 0;
    int nGameIDPrev = 0;
    std::string sSubjectPrev = "";
    std::string sAnswerPrev = "";
    std::string sAuthorPrev = "";
    std::string sAnswerFormatPrev = "";
    std::string sStatePrev = "";
    std::string sCopyrightPrev = "";
    std::string sDescriptionStatePrev = "";

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            sNamePrev = record.value("name").toString().toStdString();
            sTextPrev = record.value("text").toString().toStdString();
            nScorePrev = record.value("score").toInt();
            nGameIDPrev = record.value("gameid").toInt();
            sSubjectPrev = record.value("subject").toString().toStdString();
            sAnswerPrev = record.value("answer").toString().toStdString();
            sAuthorPrev = record.value("author").toString().toStdString();
            sAnswerFormatPrev = record.value("answer_format").toString().toStdString();
            sStatePrev = record.value("state").toString().toStdString();
            sCopyrightPrev = record.value("copyright").toString().toStdString();
            sDescriptionStatePrev = record.value("description_state").toString().toStdString();
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Quest not found"));
            return;
        }
    }

    // Update name
    if (pRequest->hasInputParam("name")) {
        std::string sName = pRequest->getInputString("name", "");
        WsjcppCore::trim(sName);
        if (sName != sNamePrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET name = :name WHERE idquest = :questid");
            query.bindValue(":name", QString::fromStdString(sName));
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            sNamePrev = sName;
            RunTasks::AddPublicEvents("quests", "Updated name of [quest#" + std::to_string(nQuestID) + "] from [" + sNamePrev + "] to [" + sName + "]");
        }
    }

    // Update gameid
    if (pRequest->hasInputParam("gameid")) {
        int nGameID = pRequest->getInputInteger("gameid", 0);
        {
            QSqlQuery query(db);
            query.prepare("SELECT * FROM games WHERE id = :id");
            query.bindValue(":id", nGameID);
            query.exec();
            if (!query.next()) {
                pRequest->sendMessageError(cmd(), WsjcppError(404, "Game not found"));
                return;
            }
        }

        if (nGameID != nGameIDPrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET gameid = :gameid WHERE idquest = :questid");
            query.bindValue(":gameid", nGameID);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::UpdateMaxScoreGame(nGameID);
            RunTasks::UpdateMaxScoreGame(nGameIDPrev);
            RunTasks::AddPublicEvents("quests", "Moved [quest#" + std::to_string(nQuestID) + "] from " + std::to_string(nGameIDPrev) + " to " + std::to_string(nGameID));
            nGameIDPrev = nGameID;
        }
    }

    // Update subject
    if (pRequest->hasInputParam("subject")) {
        std::string sSubject = pRequest->getInputString("subject", "");
        WsjcppCore::trim(sSubject);
        if (sSubject != sSubjectPrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET subject = :subject WHERE idquest = :questid");
            query.bindValue(":subject", QString::fromStdString(sSubject));
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated subject of [quest#" + std::to_string(nQuestID) + "] " + sNamePrev);
            // TODO update skills of users in future
        }
    }

    // Update text
    if (pRequest->hasInputParam("text")) {
        std::string sText = pRequest->getInputString("text", "");
        WsjcppCore::trim(sText);
        if (sText != sTextPrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET text = :text WHERE idquest = :questid");
            query.bindValue(":text", QString::fromStdString(sText));
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated text of [quest#" + std::to_string(nQuestID) + "] " + sNamePrev);
        }
    }

    // Update score
    if (pRequest->hasInputParam("score")) {
        int nScore = pRequest->getInputInteger("score", 0);
        if (nScore != nScorePrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET score = :score WHERE idquest = :questid");
            query.bindValue(":score", nScore);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated score of [quest#" + std::to_string(nQuestID) + "] " + sNamePrev + " from " + std::to_string(nScorePrev) + " to " + std::to_string(nScore));
            RunTasks::UpdateMaxScoreGame(nGameIDPrev);
            // TODO update users rating/score task
        }
    }

    // Update answer
    if (pRequest->hasInputParam("answer")) {
        std::string sAnswer = pRequest->getInputString("answer", "");
        if (sAnswer != sAnswerPrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET answer = :answer WHERE idquest = :questid");
            query.bindValue(":answer", QString::fromStdString(sAnswer));
            // TODO update md5 upper
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated answer of [quest#" + std::to_string(nQuestID) + "] " + sNamePrev);
        }
    }

    // Update author
    if (pRequest->hasInputParam("author")) {
        std::string sAuthor = pRequest->getInputString("author", "");
        if (sAuthor != sAuthorPrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET author = :author WHERE idquest = :questid");
            query.bindValue(":author", QString::fromStdString(sAuthor));
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated author of [quest#" + std::to_string(nQuestID) + "] " + sNamePrev);
        }
    }

    // Update answer format
    if (pRequest->hasInputParam("answer_format")) {
        std::string sAnswerFormat = pRequest->getInputString("answer_format", "");
        if (sAnswerFormat != sAnswerFormatPrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET answer_format = :answer_format WHERE idquest = :questid");
            query.bindValue(":answer_format", QString::fromStdString(sAnswerFormat));
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated answer format of [quest#" + std::to_string(nQuestID) + "](" + sNamePrev + ") from {" + sAnswerFormatPrev + "} to {" + sAnswerFormat + "}");
        }
    }

    // Update state
    if (pRequest->hasInputParam("state")) {
        std::string sState = pRequest->getInputString("state", "");
        if (sState != sStatePrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET state = :state WHERE idquest = :questid");
            query.bindValue(":state", QString::fromStdString(sState));
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated state of [quest#" + std::to_string(nQuestID) + "] " + sNamePrev + " from {" + sStatePrev + "} to {" + sState + "}");
        }
    }

    // Update copyright
    if (pRequest->hasInputParam("copyright")) {
        std::string sCopyright = pRequest->getInputString("copyright", "");
        if (sCopyright != sCopyrightPrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET copyright = :copyright WHERE idquest = :questid");
            query.bindValue(":copyright", QString::fromStdString(sCopyright));
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated copyright of [quest#" + std::to_string(nQuestID) + "] " + sNamePrev + " from {" + sCopyrightPrev + "} to {" + sCopyright + "}");
        }
    }

    // Update copyright
    if (pRequest->hasInputParam("description_state")) {
        std::string sDescriptionState = pRequest->getInputString("description_state", "");
        if (sDescriptionState != sDescriptionStatePrev) {
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET description_state = :description_state WHERE idquest = :questid");
            query.bindValue(":description_state", QString::fromStdString(sDescriptionState));
            query.bindValue(":questid", nQuestID);
            if (!query.exec()) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            // nothing to inform
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quests Subjects ************
// *******************************************


CmdHandlerQuestsSubjects::CmdHandlerQuestsSubjects()
    : CmdHandlerBase("quests_subjects", "Method returned list of quests by subjects") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsSubjects::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    nlohmann::json jsonSubjects = nlohmann::json::array();

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT subject, COUNT(*) as cnt FROM `quest` WHERE quest.state = :state GROUP BY subject");
    query.bindValue(":state", "open");

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonSubject;
        jsonSubject["subject"] = record.value("subject").toString().toStdString();
        jsonSubject["count"] = record.value("cnt").toInt();
        jsonSubjects.push_back(jsonSubject);
    }

    jsonResponse["data"] = jsonSubjects;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);

}

// *******************************************
// ************ Quest Hints Add **************
// *******************************************


CmdHandlerAddHint::CmdHandlerAddHint()
    : CmdHandlerBase("addhint", "Methid add hint to quest") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "quest id");
    requireStringParam("hint", "hint text");
}

// ---------------------------------------------------------------------

void CmdHandlerAddHint::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse; // TODO redesign to nlohmann::json

    // get quest id
    int nQuestId = 0;
    if (jsonRequest["questid"].is_number_integer()) {
        nQuestId = jsonRequest.at("questid");
    }

    if (nQuestId == 0) {
        // todo this check move to cmd input def
        // TODO must be check on inputDef validators
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Parameter 'questid' must be not zero"));
        return;
    }

    // hint text
    std::string sHint = "";
    if (jsonRequest["hint"].is_string()) {
        sHint = jsonRequest["hint"];
    }

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO quests_hints (questid, text, dt) VALUES (:questid, :text, NOW())");
    query.bindValue(":questid", nQuestId);
    query.bindValue(":text", QString(sHint.c_str()));
    if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
    }

    RunTasks::AddPublicEvents("quests", "Added hint for [quest#" + std::to_string(nQuestId) + "]");
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************ Quest AnswerList **************
// *******************************************

CmdHandlerAnswerList::CmdHandlerAnswerList()
    : CmdHandlerBase("answerlist", "Return user answers list") {

    TAG = "CmdHandlerAnswerList";

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("page", "Number of page"); // TODO validator
    requireIntegerParam("onpage", "How much rows on page");  // TODO validator
    optionalIntegerParam("questid", "Filter for questid");
    optionalIntegerParam("userid", "Filter for userid");
    
}

// ---------------------------------------------------------------------

void CmdHandlerAnswerList::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    int nPage = pRequest->getInputInteger("page", 0);
    jsonResponse["page"] = nPage;

    int nOnPage = pRequest->getInputInteger("onpage", 10);
    jsonResponse["onpage"] = nOnPage;

    if (nOnPage > 50) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Parameter 'onpage' could not be more then 50"));
        return;
    }

    QStringList filters;
    QMap<QString,QString> filter_values;

    if (pRequest->hasInputParam("userid")) {
        int userid = pRequest->getInputInteger("userid", 0);
        filters << "(u.id = :userid)";
        filter_values[":userid"] = userid;
    }

    if (pRequest->hasInputParam("user")) {
        std::string user = pRequest->getInputString("user", "");
        WsjcppCore::trim(user);
        filters << "(u.email like :email OR u.nick like :nick)";
        filter_values[":email"] = "%" + QString::fromStdString(user) + "%";
        filter_values[":nick"] = "%" + QString::fromStdString(user) + "%";
    }

    if (pRequest->hasInputParam("questid")) {
        int questid = pRequest->getInputInteger("questid", 0);
        filters << "(q.idquest = :questid)";
        filter_values[":questid"] = questid;
    }

    if (pRequest->hasInputParam("questname")) {
        std::string questname = pRequest->getInputString("questname", "");
        WsjcppCore::trim(questname);
        if (questname != "") {
            filters << "(q.name LIKE :questname)";
            filter_values[":questname"] = "%" + QString::fromStdString(questname) + "%";
        }
    }

    if (pRequest->hasInputParam("questsubject")) {
        std::string questsubject = pRequest->getInputString("questsubject", "");
        WsjcppCore::trim(questsubject);
        if (questsubject != "") {
            filters << "(q.subject = :questsubject)";
            filter_values[":questsubject"] = QString::fromStdString(questsubject);
        }
    }

    if (pRequest->hasInputParam("passed")) {
        std::string passed = pRequest->getInputString("passed", "");
        WsjcppCore::trim(passed);
        if (passed != "") {
            filters << "(uqa.passed = :passed)";
            filter_values[":passed"] = QString::fromStdString(passed);
        }
    }

    QString where = filters.join(" AND ");
    if (where.length() > 0) {
        where = "WHERE " + where;
    }

    // count quests
    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare("SELECT count(*) as cnt FROM users_quests_answers uqa "
            " INNER JOIN users u ON u.id = uqa.userid"
            " INNER JOIN quest q ON q.idquest = uqa.questid"
            " " + where
        );
        foreach (QString key, filter_values.keys()) {
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["count"] = record.value("cnt").toInt();
        }
    }

    // data
    nlohmann::json jsonAnswerList = nlohmann::json::array();
    {
        QSqlQuery query(db);
        query.prepare("SELECT "
            "     uqa.dt,"
            "    uqa.passed,"
            "    uqa.questid,"
            "    uqa.userid,"
            "    uqa.user_answer,"
            "    uqa.quest_answer,"
            "    uqa.levenshtein,"
            "     u.nick,"
            "    u.logo,"
            "    u.email, "
            "     q.name,"
            "    q.subject,"
            "    q.score,"
            "    q.count_user_solved"
            " FROM users_quests_answers uqa "
            " INNER JOIN users u ON u.id = uqa.userid"
            " INNER JOIN quest q ON q.idquest = uqa.questid"
            " " + where +
            " ORDER BY dt DESC "
            " LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
        );
        foreach (QString key, filter_values.keys()) {
            query.bindValue(key, filter_values.value(key));
        }
        query.exec(); // TODO check error database
        while (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonAnswer;
            jsonAnswer["dt"] = record.value("dt").toString().toStdString();
            jsonAnswer["user_answer"] = record.value("user_answer").toString().toHtmlEscaped().toStdString(); // TODO htmlspecialchars
            jsonAnswer["quest_answer"] = record.value("quest_answer").toString().toHtmlEscaped().toStdString(); // TODO htmlspecialchars
            jsonAnswer["levenshtein"] = record.value("levenshtein").toInt();
            jsonAnswer["passed"] = record.value("passed").toString().toStdString();

            nlohmann::json jsonQuest; // TODO deprecated
            jsonQuest["id"] = record.value("questid").toInt();
            jsonQuest["name"] = record.value("name").toString().toHtmlEscaped().toStdString();
            jsonQuest["score"] = record.value("score").toInt();
            jsonQuest["subject"] = record.value("subject").toString().toStdString();
            jsonQuest["count_user_solved"] = record.value("count_user_solved").toInt();
            jsonAnswer["quest"] = jsonQuest;

            nlohmann::json jsonUser; // TODO deprecated
            jsonUser["id"] = record.value("userid").toInt();
            jsonUser["logo"] = record.value("logo").toString().toStdString();
            jsonUser["nick"] = record.value("nick").toString().toHtmlEscaped().toStdString();
            jsonUser["email"] = record.value("email").toString().toHtmlEscaped().toStdString();
            jsonAnswer["user"] = jsonUser;

            jsonAnswerList.push_back(jsonAnswer);
        }
    }

    jsonResponse["data"] = jsonAnswerList;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// *********** Quest Hint Delete ************
// *******************************************


CmdHandlerDeleteHint::CmdHandlerDeleteHint()
    : CmdHandlerBase("deletehint", "Method for delete hint from quest") {
    TAG = "CmdHandlerDeleteHint";

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("hintid", "hint id");
}

// ---------------------------------------------------------------------

void CmdHandlerDeleteHint::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    int hintid = pRequest->getInputInteger("hintid", 0);
    if (hintid == 0) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Parameter 'hintid' must be not zero"));
        return;
    }

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("DELETE FROM quests_hints WHERE id = :hintid");
    query.bindValue(":hintid", hintid);
    query.exec();

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// **************** Quest Hints **************
// *******************************************

CmdHandlerHints::CmdHandlerHints()
    : CmdHandlerBase("hints", "Return list of hints") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest id");
}

// ---------------------------------------------------------------------

void CmdHandlerHints::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    int questid = pRequest->getInputInteger("questid", 0);
    if (questid == 0) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Parameter 'questid' must be not zero"));
        return;
    }

    nlohmann::json jsonHints = nlohmann::json::array();
    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM quests_hints WHERE questid = :questid");
    query.bindValue(":questid", questid);
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        int hintid = record.value("id").toInt();
        int questid2 = record.value("questid").toInt();
        QString text = record.value("text").toString();
        QString dt = record.value("dt").toString();
        nlohmann::json jsonHint;
        jsonHint["id"] = hintid;
        jsonHint["questid"] = questid2;
        jsonHint["text"] = text.toStdString(); // TODO escape html
        jsonHint["dt"] = dt.toStdString();
        jsonHints.push_back(jsonHint);
    }

    jsonResponse["data"] = jsonHints;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ********* Quests Proposal List ************
// *******************************************


CmdHandlerQuestsProposalList::CmdHandlerQuestsProposalList()
    : CmdHandlerBase("quests_proposal_list", "Quests proposal list") {
    TAG = "CmdHandlerQuestsProposalList";

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    optionalIntegerParam("onpage", "On page"); // TODO validator
    optionalIntegerParam("page", "page");  // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsProposalList::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    QStringList filters;
    QMap<QString,QString> filter_values;

    int nPage = pRequest->getInputInteger("page", 0);
    int nOnPage = pRequest->getInputInteger("onpage", 5);
    int nCount = 0;

    QSqlDatabase db = *(pDatabase->database());
    QString where = filters.join(" AND ");
    if (where.length() > 0) {
        where = "WHERE " + where;
    }

    // calculate count
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM quests_proposal " + where);
        foreach (QString key, filter_values.keys()) {
            query.bindValue(key, filter_values.value(key));
        }
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            nCount = record.value("cnt").toInt();
        }
    }

    // quests_proposal
    nlohmann::json jsonQuestsProposal = nlohmann::json::array();
    {
        QSqlQuery query(db);
        query.prepare("SELECT qp.*, u.nick, u.email, g.title as game_title  FROM quests_proposal qp "
            " LEFT JOIN users u ON u.id = qp.userid "
            " LEFT JOIN games g ON g.id = qp.gameid "
            + where + " ORDER BY created DESC LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage));
        foreach (QString key, filter_values.keys()) {
            query.bindValue(key, filter_values.value(key));
        }
        query.exec(); // TODO check error
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

            nlohmann::json jsonQuestProposal;
            jsonQuestProposal["id"] = nID;
            jsonQuestProposal["userid"] = nUserID;
            jsonQuestProposal["nick"] = sNick.toStdString();
            jsonQuestProposal["email"] = sEmail.toStdString();
            jsonQuestProposal["gameid"] = nGameID;
            jsonQuestProposal["game_title"] = sGameTitle.toStdString();
            jsonQuestProposal["name"] = sName.toStdString();
            jsonQuestProposal["score"] = nScore;
            jsonQuestProposal["created"] = sCreated.toStdString();
            jsonQuestProposal["subject"] = sSubject.toStdString();
            jsonQuestProposal["confirmed"] = nConfirmed;

            jsonQuestsProposal.push_back(jsonQuestProposal);
        }
    }

    jsonResponse["data"] = jsonQuestsProposal;
    jsonResponse["onpage"] = nOnPage;
    jsonResponse["page"] = nPage;
    jsonResponse["count"] = nCount;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);

}


CmdHandlerQuestsFilesUpload::CmdHandlerQuestsFilesUpload()
    : CmdHandlerBase("quests_files_upload", "Update the quest ") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("quest_uuid", "Quest UUID")
        .addValidator(new WsjcppValidatorUUID());
    requireStringParam("file_base64", "");
    requireStringParam("file_name", "");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsFilesUpload::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonResponse;
    QSqlDatabase db = *(pDatabase->database());

    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    std::string sPublicFolder = pGlobalSettings->get("web_public_folder").getDirPathValue();
    // std::string sPublicFolderURL = pGlobalSettings->get("web_public_folder_url").getDirPathValue();

    std::string sQuestUUID = pRequest->getInputString("quest_uuid", "");
    std::string sFileBase64 = pRequest->getInputString("file_base64", "");
    std::string sFileName = pRequest->getInputString("file_name", "");
    int nQuestID = 0;

    {
        QSqlQuery query(db);
        query.prepare("SELECT idquest FROM quest WHERE uuid = :questuuid");
        query.bindValue(":questuuid", QString::fromStdString(sQuestUUID));
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord quest = query.record();
            nQuestID = quest.value("idquest").toInt();
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Game not found"));
            return;
        }
    }

    QByteArray baFileBase64;
    QString sImagePngBase64 = QString::fromStdString(sFileBase64);
    baFileBase64.append(sImagePngBase64);
    // TODO replace decode base64 to std
    QByteArray baFile = QByteArray::fromBase64(baFileBase64); // .fromBase64(baImagePNGBase64);

    if (baFile.size() == 0) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Could not decode base64"));
        return;
    }
    
    if (!WsjcppCore::dirExists(sPublicFolder + "/quests/")) {
        WsjcppCore::makeDir(sPublicFolder + "/quests/");
    }
    // TODO replace in filename all dots
    std::string sFileUuid = WsjcppCore::createUuid();
    sFileUuid = WsjcppCore::toUpper(sFileUuid);
    std::string sPath = sPublicFolder + "/quests/" + WsjcppCore::toUpper(sQuestUUID) + "_" + sFileUuid;
    
    int nFileSize = baFile.size();
    FILE * pFile = fopen(sPath.c_str(), "wb");
    fwrite (baFile.constData(), sizeof(char), nFileSize, pFile);
    fclose (pFile);
    
    // insert to user tries
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO quests_files(uuid, questid, filename, size, dt, filepath) "
                      "VALUES(:uuid, :questid, :filename, :size, NOW(), :filepath)");
        
        query.bindValue(":uuid", QString::fromStdString(sFileUuid));
        query.bindValue(":questid", nQuestID);
        query.bindValue(":filename", QString::fromStdString(sFileName));
        query.bindValue(":size", nFileSize);
        query.bindValue(":filepath", QString::fromStdString("public/quests/" + WsjcppCore::toUpper(sQuestUUID) + "_" + sFileUuid));

        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}