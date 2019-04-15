#include <cmd_handlers_quests.h>
#include <runtasks.h>
#include <utils_logger.h>
#include <utils.h>
#include <md5.h>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <employ_notify.h>

// *******************************************
// *************** Quest List ****************
// *******************************************

CmdHandlerQuests::CmdHandlerQuests()
    : CmdHandlerBase("quests", "Method will be returned quest list"){

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    optionalStringParam("subject", "Filter by subject"); // TODO validator
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
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
}

// ---------------------------------------------------------------------

void CmdHandlerQuest::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

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
            "    q.idquest, "
            "    q.gameid, "
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
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonQuest;
            jsonQuest["id"] = record.value("idquest").toInt();
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
                if(!query_game.exec()){
                    pRequest->sendMessageError(cmd(), Error(500, query_game.lastError().text().toStdString()));
                    return;
                }
                if (query_game.next()) {
                    QSqlRecord record_game = query_game.record();
                    int nGameID = record_game.value("id").toInt();
                    jsonGame["id"] = nGameID;
                    jsonGame["title"] = record_game.value("title").toString().toStdString();
                    jsonGame["logo"] = QString(sBaseGamesURL + QString::number(nGameID) + ".png").toStdString();
                }else{
                    pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
                    return;
                }
                jsonResponse["game"] = jsonGame;
            }


            // files
            {
                auto jsonFiles = nlohmann::json::array();
                QSqlQuery query_files(db);
                query_files.prepare("SELECT * FROM quests_files WHERE questid = :questid");
                query_files.bindValue(":questid", nQuestID);
                if(!query_files.exec()){
                    pRequest->sendMessageError(cmd(), Error(500, query_files.lastError().text().toStdString()));
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

            // hints
            {
                auto jsonHints = nlohmann::json::array();

                QSqlQuery query_hints(db);
                query_hints.prepare("SELECT * FROM quests_hints WHERE questid = :questid");
                query_hints.bindValue(":questid", nQuestID);
                if (!query_hints.exec()) {
                    pRequest->sendMessageError(cmd(), Error(500, query_hints.lastError().text().toStdString()));
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

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
    requireStringParam("answer", "Answer");

}

// ---------------------------------------------------------------------

void CmdHandlerQuestPass::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
    int nLevenshtein = UtilsLevenshtein::distance(sUserAnswer.toUpper().toStdString(), sQuestAnswer.toUpper().toStdString());


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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        pServerInfo->incrementQuestsAttempt();
    }

    if(!bPassed){
        pRequest->sendMessageError(cmd(), Error(403, "Answer incorrect. Levenshtein distance: " + QString::number(nLevenshtein).toStdString()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }
    pServerInfo->incrementQuestsCompleted();

    EmployNotify *pEmployNotify = findEmploy<EmployNotify>();

    pEmployNotify->notifyInfo("quests", "User #" + std::to_string(nUserID) + "  " + sNick.toStdString()
                              + " passed quest #" + std::to_string(nQuestID) + " " + sQuestName.toStdString());

    RunTasks::UpdateUserRating(nUserID);
    RunTasks::UpdateQuestSolved(nQuestID);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Create ***************
// *******************************************

CmdHandlerCreateQuest::CmdHandlerCreateQuest()
    : CmdHandlerBase("createquest", "Method will be create new quest"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireStringParam("uuid", "Global Identificator of the quest")
        .addValidator(new ValidatorUUID());

    requireIntegerParam("gameid", "Which game included this quest");
    requireStringParam("name", "Name of the quest");
    requireStringParam("text", "Description of the quest");
    requireIntegerParam("score", "How much append to user score after solve quest by them").minval(1).maxval(1000);
    // TODO validator score

    // TODO from database init
    QStringList questTypes;
    questTypes << "trivia";
    requireStringParam("author", "Author of the quest");
    requireStringParam("subject", "Subject must be one from types (look types)"); // TODO validator
    requireStringParam("answer", "Answer for the quest");
    requireStringParam("answer_format", "Answer format for the quest");
    requireStringParam("state", "State of the quest, can be: open, broken, closed"); // TODO validator
    requireStringParam("description_state", "You can add some descriptions for quest state");
}

// ---------------------------------------------------------------------

void CmdHandlerCreateQuest::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QString sUUID = jsonRequest["uuid"].toString().trimmed();
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE uuid = :uuid");
        query.bindValue(":uuid", sUUID);
        query.exec();
        if (query.next()) {
            pRequest->sendMessageError(cmd(), Error(403, "Quest with uuid {" + sUUID.toStdString() + "} already exists"));
            return;
        }
    }

    int nGameID = jsonRequest["gameid"].toInt();
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE id = :id");
        query.bindValue(":id", nGameID);
        query.exec();
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
            return;
        }
    }

    QString sName = jsonRequest["name"].toString().trimmed();
    /*if(sName.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Name could not be empty"));
        return;
    }*/

    QString sText = jsonRequest["text"].toString().trimmed();
    int nScore = jsonRequest["score"].toInt();
    QString sSubject = jsonRequest["subject"].toString().trimmed();
    QString sAnswer = jsonRequest["answer"].toString().trimmed();
    QString sAuthor = jsonRequest["author"].toString().trimmed();
    QString sAnswerFormat = jsonRequest["answer_format"].toString().trimmed();
    QString sState = jsonRequest["state"].toString().trimmed();
    QString sCopyright = jsonRequest["copyright"].toString().trimmed();
    QString sDescriptionState = jsonRequest["description_state"].toString().trimmed();

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
    query.bindValue(":uuid", sUUID);
    query.bindValue(":gameid", nGameID);
    query.bindValue(":name", sName);
    query.bindValue(":text", sText);
    query.bindValue(":answer", sAnswer);
    std::string sAnswerUpperMd5_ = md5(sAnswer.toUpper().toStdString());
    QString sAnswerUpperMd5 = QString::fromStdString(sAnswerUpperMd5_);
    query.bindValue(":answer_upper_md5", sAnswerUpperMd5);
    query.bindValue(":answer_format", sAnswerFormat);
    query.bindValue(":score", nScore);
    query.bindValue(":author", sAuthor);
    query.bindValue(":subject", sSubject);
    query.bindValue(":state", sState);
    query.bindValue(":description_state", sDescriptionState);
    query.bindValue(":copyright", sCopyright);
    query.bindValue(":count_user_solved", 0);

    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
        return;
    }
    pServerInfo->incrementQuests();


    int rowid = query.lastInsertId().toInt();
    jsonResponse["questid"] = rowid;

    RunTasks::AddPublicEvents("quests", "New quest #" + QString::number(rowid) + " " + sName + " (subject: " + sSubject + ")");
    RunTasks::UpdateMaxScoreGame(nGameID);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Delete ***************
// *******************************************


CmdHandlerQuestDelete::CmdHandlerQuestDelete()
    : CmdHandlerBase("quest_delete", "Method for delete quest"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestDelete::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    int questid = jsonRequest["questid"].toInt();
    QString sName = "";
    QString sSubject = "";
    QSqlDatabase db = *(pDatabase->database());
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", questid);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            sName = record.value("name").toString();
            sSubject = record.value("subject").toString();
        }else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
            return;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", questid);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    // remove from users_quests_answers
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM users_quests_answers WHERE questid = :questid");
        query.bindValue(":questid", questid);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    // remove from users_quests
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM users_quests WHERE questid = :questid");
        query.bindValue(":questid", questid);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }
    pServerInfo->decrementQuests();

    RunTasks::AddPublicEvents("quests", "Removed quest #" + QString::number(questid) + " " + sName + " (subject: " + sSubject + ")");

    // todo recalculate rating/score for users how solved this quest

    jsonResponse["subject"] = sSubject.toStdString();
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Proposal ***************
// *******************************************


CmdHandlerQuestProposal::CmdHandlerQuestProposal()
    : CmdHandlerBase("quest_proposal", "Add quest proposal"){
    TAG = "CmdQuestProposalHandler";

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("gameid", "Which game included this quest");
    requireStringParam("name", "Name of the quest");
    requireStringParam("text", "Description of the quest");
    requireIntegerParam("score", "How much append to user score after solve quest by them").minval(1).maxval(1000);

    QStringList questTypes; // TODO fill from database or use employees
    questTypes << "trivia";
    requireStringParam("author", "Author of the quest");
    requireStringParam("subject", "Subject must be one from types (look types)"); // TODO validator
    requireStringParam("answer", "Answer for the quest");
    requireStringParam("answer_format", "Answer format for the quest");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestProposal::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QSqlDatabase db = *(pDatabase->database());

    IUserToken *pUserToken = pRequest->userToken();
    int nUserID = 0;
    QString sUserEmail = "";
    if(pUserToken != NULL) {
        nUserID = pUserToken->userid();
        sUserEmail = pUserToken->email();
    }

    // check the user already sended quest

    int nGameID = jsonRequest["gameid"].toInt();
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE id = :id");
        query.bindValue(":id", nGameID);
        query.exec();
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
            return;
        }
    }

    QString sName = jsonRequest["name"].toString().trimmed();
    if(sName.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Name could not be empty"));
        return;
    }

    QString sText = jsonRequest["text"].toString().trimmed();

    int nScore = jsonRequest["score"].toInt();
    QString sSubject = jsonRequest["subject"].toString().trimmed();
    QString sAnswer = jsonRequest["answer"].toString().trimmed();

    if(sAnswer.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Answer could not be empty"));
        return;
    }

    QString sAuthor = jsonRequest["author"].toString().trimmed();

    if(sAuthor.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Author could not be empty"));
        return;
    }

    QString sAnswerFormat = jsonRequest["answer_format"].toString().trimmed();

    if(sAnswerFormat.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Answer Format could not be empty"));
        return;
    }

    QString sCopyright = jsonRequest["copyright"].toString().trimmed();

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
    query.bindValue(":name", sName);
    query.bindValue(":text", sText);
    query.bindValue(":answer", sAnswer);
    query.bindValue(":answer_format", sAnswerFormat);
    query.bindValue(":score", nScore);
    query.bindValue(":author", sAuthor);
    query.bindValue(":subject", sSubject);
    query.bindValue(":copyright", sCopyright);
    query.bindValue(":confirmed", 0);

    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
        return;
    }
    // pMemoryCacheServerInfo->incrementQuests();


    int nQuestProposalId = query.lastInsertId().toInt();
    jsonResponse["questid"] = nQuestProposalId;

    std::string sMailToAdmin = pSettings->getSettString("mail_system_message_admin_email").toStdString();
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
    : CmdHandlerBase("quest_statistics", "Mehtod will be return quest public statistics"){

    TAG = "CmdHandlerQuestStatistics";

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestStatistics::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int nQuestID = jsonRequest["questid"].toInt();

    // check quest exists
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }

        if (!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
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
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["tries"] = record.value("cnt").toInt();
        }else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
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
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["solved"] = record.value("cnt").toInt();
        }else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
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

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        auto jsonUsers = nlohmann::json::array();
        while(query.next()) {
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
    : CmdHandlerBase("quest_update", "Update the quest info"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
    optionalStringParam("name", "Name of the quest");
    optionalStringParam("gameid", "Which game included this quest");
    optionalStringParam("text", "Description of the quest");
    optionalIntegerParam("score", "How much append to user score after solve quest by them").minval(1).maxval(1000); // TODO validator

    // TODO from database init
    QStringList questTypes;
    questTypes << "trivia";

    optionalStringParam("subject", "Subject must be one from types (look types)");  // TODO validator
    optionalStringParam("answer", "Answer for the quest");
    optionalStringParam("answer_format", "Answer format for the quest");
    optionalStringParam("state", "State of the quest, can be: open, broken, closed"); // TODO validator
    optionalStringParam("description_state", "You can add some descriptions for quest state");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestUpdate::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int nQuestID = jsonRequest["questid"].toInt();
    QString sNamePrev = "";
    QString sTextPrev = "";
    int nScorePrev = 0;
    int nGameIDPrev = 0;
    QString sSubjectPrev = "";
    QString sAnswerPrev = "";
    QString sAuthorPrev = "";
    QString sAnswerFormatPrev = "";
    QString sStatePrev = "";
    QString sCopyrightPrev = "";
    QString sDescriptionStatePrev = "";

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            sNamePrev = record.value("name").toString();
            sTextPrev = record.value("text").toString();
            nScorePrev = record.value("score").toInt();
            nGameIDPrev = record.value("gameid").toInt();
            sSubjectPrev = record.value("subject").toString();
            sAnswerPrev = record.value("answer").toString();
            sAuthorPrev = record.value("author").toString();
            sAnswerFormatPrev = record.value("answer_format").toString();
            sStatePrev = record.value("state").toString();
            sCopyrightPrev = record.value("copyright").toString();
            sDescriptionStatePrev = record.value("description_state").toString();
        }else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
            return;
        }
    }

    // Update name
    if(jsonRequest.contains("name")){
        QString sName = jsonRequest["name"].toString().trimmed();
        if(sName != sNamePrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET name = :name WHERE idquest = :questid");
            query.bindValue(":name", sName);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            sNamePrev = sName;
            RunTasks::AddPublicEvents("quests", "Updated name of quest #" + QString::number(nQuestID) + " from [" + sNamePrev + "] to [" + sName + "]");
        }
    }

    // Update gameid
    if(jsonRequest.contains("gameid")){
        int nGameID = jsonRequest["gameid"].toInt();
        {
            QSqlQuery query(db);
            query.prepare("SELECT * FROM games WHERE id = :id");
            query.bindValue(":id", nGameID);
            query.exec();
            if (!query.next()) {
                pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
                return;
            }
        }

        if(nGameID != nGameIDPrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET gameid = :gameid WHERE idquest = :questid");
            query.bindValue(":gameid", nGameID);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::UpdateMaxScoreGame(nGameID);
            RunTasks::UpdateMaxScoreGame(nGameIDPrev);
            RunTasks::AddPublicEvents("quests", "Moved quest #" + QString::number(nQuestID) + " from " + QString::number(nGameIDPrev) + " to " + QString::number(nGameID));
            nGameIDPrev = nGameID;
        }
    }

    // Update subject
    if(jsonRequest.contains("subject")){
        QString sSubject = jsonRequest["subject"].toString().trimmed();
        if(sSubject != sSubjectPrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET subject = :subject WHERE idquest = :questid");
            query.bindValue(":subject", sSubject);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated subject of quest #" + QString::number(nQuestID) + " " + sNamePrev);
            // TODO update skills of users in future
        }
    }

    // Update text
    if(jsonRequest.contains("text")){
        QString sText = jsonRequest["text"].toString().trimmed();
        if(sText != sTextPrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET text = :text WHERE idquest = :questid");
            query.bindValue(":text", sText);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated text of quest #" + QString::number(nQuestID) + " " + sNamePrev);
        }
    }

    // Update score
    if(jsonRequest.contains("score")){
        int nScore = jsonRequest["score"].toInt();
        if(nScore != nScorePrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET score = :score WHERE idquest = :questid");
            query.bindValue(":score", nScore);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated score of quest #" + QString::number(nQuestID) + " " + sNamePrev + " from " + QString::number(nScorePrev) + " to " + QString::number(nScore));
            RunTasks::UpdateMaxScoreGame(nGameIDPrev);
            // TODO update users reting/score
        }
    }

    // Update answer
    if(jsonRequest.contains("answer")){
        QString sAnswer = jsonRequest["answer"].toString();
        if(sAnswer != sAnswerPrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET answer = :answer WHERE idquest = :questid");
            query.bindValue(":answer", sAnswer);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated answer of quest #" + QString::number(nQuestID) + " " + sNamePrev);
        }
    }

    // Update author
    if(jsonRequest.contains("author")){
        QString sAuthor = jsonRequest["author"].toString();
        if(sAuthor != sAuthorPrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET author = :author WHERE idquest = :questid");
            query.bindValue(":author", sAuthor);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated author of quest #" + QString::number(nQuestID) + " " + sNamePrev);
        }
    }

    // Update answer format
    if(jsonRequest.contains("answer_format")){
        QString sAnswerFormat = jsonRequest["answer_format"].toString();
        if(sAnswerFormat != sAnswerFormatPrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET answer_format = :answer_format WHERE idquest = :questid");
            query.bindValue(":answer_format", sAnswerFormat);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated answer format of quest #" + QString::number(nQuestID) + " " + sNamePrev + " from [" + sAnswerFormatPrev + "] to [" + sAnswerFormat + "]");
        }
    }

    // Update state
    if(jsonRequest.contains("state")){
        QString sState = jsonRequest["state"].toString();
        if(sState != sStatePrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET state = :state WHERE idquest = :questid");
            query.bindValue(":state", sState);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated state of quest #" + QString::number(nQuestID) + " " + sNamePrev + " from [" + sStatePrev + "] to [" + sState + "]");
        }
    }

    // Update copyright
    if(jsonRequest.contains("copyright")){
        QString sCopyright = jsonRequest["copyright"].toString();
        if(sCopyright != sCopyrightPrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET copyright = :copyright WHERE idquest = :questid");
            query.bindValue(":copyright", sCopyright);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
                return;
            }
            RunTasks::AddPublicEvents("quests", "Updated copyright of quest #" + QString::number(nQuestID) + " " + sNamePrev + " from [" + sCopyrightPrev + "] to [" + sCopyright + "]");
        }
    }

    // Update copyright
    if(jsonRequest.contains("description_state")){
        QString sDescriptionState = jsonRequest["description_state"].toString();
        if(sDescriptionState != sDescriptionStatePrev){
            QSqlQuery query(db);
            query.prepare("UPDATE quest SET description_state = :description_state WHERE idquest = :questid");
            query.bindValue(":description_state", sDescriptionState);
            query.bindValue(":questid", nQuestID);
            if (!query.exec()){
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
    : CmdHandlerBase("quests_subjects", "Method returned list of quests by subjects"){

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsSubjects::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    auto jsonSubjects = nlohmann::json::array();

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT subject, COUNT(*) as cnt FROM `quest` WHERE quest.state = :state GROUP BY subject");
    query.bindValue(":state", "open");

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
    : CmdHandlerBase("addhint", "Methid add hint to quest"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "quest id");
    requireStringParam("hint", "hint text");
}

// ---------------------------------------------------------------------

void CmdHandlerAddHint::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse; // TODO redesign to nlohmann::json

    // get quest id
    int nQuestId = 0;
    if(jsonRequest["questid"].is_number_integer()){
        nQuestId = jsonRequest.at("questid");
    }

    if(nQuestId == 0){
        // todo this check move to cmd input def
        // TODO must be check on inputDef validators
        pRequest->sendMessageError(cmd(), Error(400, "Parameter 'questid' must be not zero"));
        return;
    }

    // hint text
    std::string sHint = "";
    if(jsonRequest["hint"].is_string()){
        sHint = jsonRequest["hint"];
    }

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO quests_hints (questid, text, dt) VALUES (:questid, :text, NOW())");
    query.bindValue(":questid", nQuestId);
    query.bindValue(":text", QString(sHint.c_str()));
    if(!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
    }

    RunTasks::AddPublicEvents("quests", "Added hint for quest #" + QString::number(nQuestId));
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************ Quest AnswerList **************
// *******************************************

CmdHandlerAnswerList::CmdHandlerAnswerList()
    : CmdHandlerBase("answerlist", "Return user answers list"){

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

void CmdHandlerAnswerList::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    int nPage = jsonRequest["page"].toInt();
    jsonResponse["page"] = nPage;

    int nOnPage = jsonRequest["onpage"].toInt();
    jsonResponse["onpage"] = nOnPage;

    if(nOnPage > 50){
        pRequest->sendMessageError(cmd(), Error(400, "Parameter 'onpage' could not be more then 50"));
        return;
    }

    QStringList filters;
    QMap<QString,QString> filter_values;

    if(jsonRequest.contains("userid")){
        int userid = jsonRequest["userid"].toInt();
        filters << "(u.id = :userid)";
        filter_values[":userid"] = userid;
    }

    if(jsonRequest.contains("user")){
        QString user = jsonRequest["user"].toString().trimmed();
        filters << "(u.email like :email OR u.nick like :nick)";
        filter_values[":email"] = "%" + user + "%";
        filter_values[":nick"] = "%" + user + "%";
    }

    if(jsonRequest.contains("questid")){
        int questid = jsonRequest["questid"].toInt();
        filters << "(q.idquest = :questid)";
        filter_values[":questid"] = questid;
    }

    if(jsonRequest.contains("questname")){
        QString questname = jsonRequest["questname"].toString().trimmed();
        if(questname != ""){
            filters << "(q.name LIKE :questname)";
            filter_values[":questname"] = "%" + questname + "%";
        }
    }

    if(jsonRequest.contains("questsubject")){
        QString questsubject = jsonRequest["questsubject"].toString().trimmed();
        if(questsubject != ""){
            filters << "(q.subject = :questsubject)";
            filter_values[":questsubject"] = questsubject;
        }
    }

    if(jsonRequest.contains("passed")){
        QString passed = jsonRequest["passed"].toString().trimmed();
        if(passed != ""){
            filters << "(uqa.passed = :passed)";
            filter_values[":passed"] = passed;
        }
    }

    QString where = filters.join(" AND ");
    if(where.length() > 0){
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
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["count"] = record.value("cnt").toInt();
        }
    }

    // data
    auto jsonAnswerList = nlohmann::json::array();
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
        foreach(QString key, filter_values.keys() ){
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
    : CmdHandlerBase("deletehint", "Method for delete hint from quest"){
    TAG = "CmdHandlerDeleteHint";

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("hintid", "hint id");
}

// ---------------------------------------------------------------------

void CmdHandlerDeleteHint::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    int hintid = jsonRequest["hintid"].toInt();
    if(hintid == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Parameter 'hintid' must be not zero"));
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
    : CmdHandlerBase("hints", "Return list of hints"){

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("questid", "Quest id");
}

// ---------------------------------------------------------------------

void CmdHandlerHints::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    int questid = jsonRequest["questid"].toInt();
    if(questid == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Parameter 'questid' must be not zero"));
        return;
    }

    auto jsonHints = nlohmann::json::array();
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
    : CmdHandlerBase("quests_proposal_list", "Quests proposal list"){
    TAG = "CmdHandlerQuestsProposalList";

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    optionalIntegerParam("onpage", "On page"); // TODO validator
    optionalIntegerParam("page", "page");  // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsProposalList::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        if(query.next()) {
            QSqlRecord record = query.record();
            nCount = record.value("cnt").toInt();
        }
    }

    // quests_proposal
    auto jsonQuestsProposal = nlohmann::json::array();
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
