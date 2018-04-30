#include <cmd_handlers_quests.h>
#include <runtasks.h>
#include <log.h>
#include <utils.h>
#include <employ_settings.h>
#include <QCryptographicHash>
#include <employ_database.h>
#include <employ_server_info.h>

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
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

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
        pServerInfo->incrementQuestsAttempt();
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
    pServerInfo->incrementQuestsCompleted();


    RunTasks::AddPublicEvents("quests", "User #" + QString::number(nUserID) + "  " + sNick
                              + " passed quest #" + QString::number(nQuestID) + " " + sQuestName);

    RunTasks::UpdateUserRating(nUserID);
    RunTasks::UpdateQuestSolved(nQuestID);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Create ***************
// *******************************************

CmdHandlerCreateQuest::CmdHandlerCreateQuest()
    : CmdHandlerBase("createquest", "Method will be create new quest"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the quest"));
    m_vInputs.push_back(CmdInputDef("gameid").integer_().required().description("Which game included this quest"));
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the quest"));
    m_vInputs.push_back(CmdInputDef("text").string_().required().description("Description of the quest"));
    m_vInputs.push_back(CmdInputDef("score").integer_().minval(1).maxval(1000).required().description("How much append to user score after solve quest by them"));

    // TODO from database init
    QStringList questTypes;
    questTypes << "trivia";
    m_vInputs.push_back(CmdInputDef("author").string_().required().description("Author of the quest"));
    m_vInputs.push_back(CmdInputDef("subject").string_().required().description("Subject must be one from types (look types)"));
    m_vInputs.push_back(CmdInputDef("answer").string_().required().description("Answer for the quest"));
    m_vInputs.push_back(CmdInputDef("answer_format").string_().required().description("Answer format for the quest"));
    m_vInputs.push_back(CmdInputDef("state").string_().required().description("State of the quest, can be: open, broken, closed"));
    m_vInputs.push_back(CmdInputDef("description_state").string_().required().description("You can add some descriptions for quest state"));
}

// ---------------------------------------------------------------------

void CmdHandlerCreateQuest::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QString sUUID = jsonRequest["uuid"].toString().trimmed();
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE uuid = :uuid");
        query.bindValue(":uuid", sUUID);
        query.exec();
        if (query.next()) {
            pRequest->sendMessageError(cmd(), Error(403, "Quest with uuid {" + sUUID + "} already exists"));
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
        "		uuid,"
        "		gameid,"
        "		name,"
        "		text,"
        "		answer,"
        "		answer_upper_md5,"
        "		answer_format,"
        "		score,"
        "		author,"
        "		subject,"
        "		state,"
        "		description_state,"
        "		copyright,"
        "		count_user_solved,"
        "		date_change,"
        "		date_create"
        "	)"
        "	VALUES("
        "		:uuid,"
        "		:gameid,"
        "		:name,"
        "		:text,"
        "		:answer,"
        "		:answer_upper_md5,"
        "		:answer_format,"
        "		:score,"
        "		:author,"
        "		:subject,"
        "		:state,"
        "		:description_state,"
        "		:copyright,"
        "		:count_user_solved,"
        "		NOW(),"
        "		NOW()"
        "	)");
    query.bindValue(":uuid", sUUID);
    query.bindValue(":gameid", nGameID);
    query.bindValue(":name", sName);
    query.bindValue(":text", sText);
    query.bindValue(":answer", sAnswer);
    QString sAnswerUpperMd5 = QString(QCryptographicHash::hash(sAnswer.toUpper().toUtf8(),QCryptographicHash::Md5).toHex());
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
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    pServerInfo->incrementQuests();


    int rowid = query.lastInsertId().toInt();
    jsonResponse["questid"] = QJsonValue(rowid);

    RunTasks::AddPublicEvents("quests", "New quest #" + QString::number(rowid) + " " + sName + " (subject: " + sSubject + ")");
    RunTasks::UpdateMaxScoreGame(nGameID);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Delete ***************
// *******************************************


CmdHandlerQuestDelete::CmdHandlerQuestDelete()
    : CmdHandlerBase("quest_delete", "Method for delete quest"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest ID"));
}

// ---------------------------------------------------------------------

void CmdHandlerQuestDelete::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int questid = jsonRequest["questid"].toInt();
    QString sName = "";
    QString sSubject = "";
    QSqlDatabase db = *(pDatabase->database());
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", questid);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    // remove from users_quests_answers
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM users_quests_answers WHERE questid = :questid");
        query.bindValue(":questid", questid);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    // remove from users_quests
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM users_quests WHERE questid = :questid");
        query.bindValue(":questid", questid);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }
    pServerInfo->decrementQuests();

    RunTasks::AddPublicEvents("quests", "Removed quest #" + QString::number(questid) + " " + sName + " (subject: " + sSubject + ")");

    // todo recalculate rating/score for users how solved this quest

    jsonResponse["subject"] = sSubject;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Proposal ***************
// *******************************************


CmdHandlerQuestProposal::CmdHandlerQuestProposal()
    : CmdHandlerBase("quest_proposal", "Add quest proposal"){
    TAG = "CmdQuestProposalHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("gameid").integer_().required().description("Which game included this quest"));
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the quest"));
    m_vInputs.push_back(CmdInputDef("text").string_().required().description("Description of the quest"));
    m_vInputs.push_back(CmdInputDef("score").integer_().minval(1).maxval(1000).required().description("How much append to user score after solve quest by them"));

    QStringList questTypes; // TODO fill from database or use employees
    questTypes << "trivia";
    m_vInputs.push_back(CmdInputDef("author").string_().required().description("Author of the quest"));
    m_vInputs.push_back(CmdInputDef("subject").string_().required().description("Subject must be one from types (look types)"));
    m_vInputs.push_back(CmdInputDef("answer").string_().required().description("Answer for the quest"));
    m_vInputs.push_back(CmdInputDef("answer_format").string_().required().description("Answer format for the quest"));
}

// ---------------------------------------------------------------------

void CmdHandlerQuestProposal::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

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
        "		userid,"
        "		gameid,"
        "		name,"
        "		text,"
        "		answer,"
        "		answer_format,"
        "		score,"
        "		author,"
        "		subject,"
        "		copyright,"
        "		created,"
        "		updated,"
        "		confirmed"
        "	)"
        "	VALUES("
        "		:userid,"
        "		:gameid,"
        "		:name,"
        "		:text,"
        "		:answer,"
        "		:answer_format,"
        "		:score,"
        "		:author,"
        "		:subject,"
        "		:copyright,"
        "		NOW(),"
        "		NOW(),"
        "		:confirmed"
        "	)");
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
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    // pMemoryCacheServerInfo->incrementQuests();


    int nQuestProposalId = query.lastInsertId().toInt();
    jsonResponse["questid"] = QJsonValue(nQuestProposalId);

    QString sMailToAdmin = pSettings->getSettString("mail_system_message_admin_email");
    QString sMessageSubject = "Quest Proposal (FreeHackQuest)";
    QString sContext = "Quest Proposal \n"
                       "UserID: " + QString::number(nUserID) + "\n"
                       "From: " + sUserEmail + "\n"
                       "QuestProposalId: #" + QString::number(nQuestProposalId) + "\n";

    RunTasks::MailSend(pRequest->server(), sMailToAdmin, sMessageSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ************** Quest Statistics ***********
// *******************************************

CmdHandlerQuestStatistics::CmdHandlerQuestStatistics()
    : CmdHandlerBase("quest_statistics", "Mehtod will be return quest public statistics"){

    TAG = "CmdHandlerQuestStatistics";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
}

// ---------------------------------------------------------------------

void CmdHandlerQuestStatistics::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    QSqlDatabase db = *(pDatabase->database());

    int nQuestID = jsonRequest["questid"].toInt();

    // check quest exists
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        QJsonArray users;
        while(query.next()) {
            QSqlRecord record = query.record();
            QJsonObject user;
            user["userid"] = record.value("id").toInt();
            user["logo"] = record.value("logo").toString().toHtmlEscaped();
            user["nick"] = record.value("nick").toString().toHtmlEscaped();
            user["university"] = record.value("university").toString().toHtmlEscaped();
            users.append(user);
        }
        jsonResponse["users"] = users;
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

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
    m_vInputs.push_back(CmdInputDef("name").string_().optional().description("Name of the quest"));
    m_vInputs.push_back(CmdInputDef("gameid").integer_().optional().description("Which game included this quest"));
    m_vInputs.push_back(CmdInputDef("text").string_().optional().description("Description of the quest"));
    m_vInputs.push_back(CmdInputDef("score").integer_().minval(1).maxval(1000).optional().description("How much append to user score after solve quest by them"));

    // TODO from database init
    QStringList questTypes;
    questTypes << "trivia";

    m_vInputs.push_back(CmdInputDef("subject").string_().optional().description("Subject must be one from types (look types)"));
    m_vInputs.push_back(CmdInputDef("answer").string_().optional().description("Answer for the quest"));
    m_vInputs.push_back(CmdInputDef("answer_format").string_().optional().description("Answer format for the quest"));
    m_vInputs.push_back(CmdInputDef("state").string_().optional().description("State of the quest, can be: open, broken, closed"));
    m_vInputs.push_back(CmdInputDef("description_state").string_().optional().description("You can add some descriptions for quest state"));
}

// ---------------------------------------------------------------------

void CmdHandlerQuestUpdate::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
                pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    // m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
    // m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsSubjects::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QJsonArray subjects;
    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT subject, COUNT(*) as cnt FROM `quest` WHERE quest.state = :state GROUP BY subject");
    query.bindValue(":state", "open");

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        QJsonObject subject;
        subject["subject"] = record.value("subject").toString();
        subject["count"] = record.value("cnt").toInt();
        subjects.push_back(subject);
    }

    jsonResponse["data"] = subjects;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);

}

// *******************************************
// ************ Quest Hints Add **************
// *******************************************


CmdHandlerAddHint::CmdHandlerAddHint()
    : CmdHandlerBase("addhint", "Methid add hint to quest"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("quest id"));
    m_vInputs.push_back(CmdInputDef("hint").required().string_().description("hint text"));
}

// ---------------------------------------------------------------------

void CmdHandlerAddHint::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();
    QJsonObject jsonResponse; // TODO redesign to nlohmann::json

    // get quest id
    int nQuestId = 0;
    if(jsonRequest["questid"].is_number_integer()){
        nQuestId = jsonRequest.at("questid");
    }

    if(nQuestId == 0){
        // todo this check move to cmd input def
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
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

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
    m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows on page"));
    m_vInputs.push_back(CmdInputDef("questid").optional().integer_().description("Filter for questid"));
    m_vInputs.push_back(CmdInputDef("userid").optional().integer_().description("Filter for userid"));
}

// ---------------------------------------------------------------------

void CmdHandlerAnswerList::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int nPage = jsonRequest["page"].toInt();
    jsonResponse["page"] = nPage;

    int nOnPage = jsonRequest["onpage"].toInt();
    jsonResponse["onpage"] = nOnPage;

    if(nOnPage > 50){
        pRequest->sendMessageError(cmd(), Errors::OnPageCouldNotBeMoreThen50());
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
    QJsonArray answerlist;
    {
        QSqlQuery query(db);
        query.prepare("SELECT "
            " 	uqa.dt,"
            "	uqa.passed,"
            "	uqa.questid,"
            "	uqa.userid,"
            "	uqa.user_answer,"
            "	uqa.quest_answer,"
            "	uqa.levenshtein,"
            " 	u.nick,"
            "	u.logo,"
            "	u.email, "
            " 	q.name,"
            "	q.subject,"
            "	q.score,"
            "	q.count_user_solved"
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
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            QJsonObject answer;
            answer["dt"] = record.value("dt").toString();
            answer["user_answer"] = record.value("user_answer").toString().toHtmlEscaped(); // TODO htmlspecialchars
            answer["quest_answer"] = record.value("quest_answer").toString().toHtmlEscaped(); // TODO htmlspecialchars
            answer["levenshtein"] = record.value("levenshtein").toInt();
            answer["passed"] = record.value("passed").toString();

            QJsonObject quest; // TODO deprecated
            quest["id"] = record.value("questid").toInt();
            quest["name"] = record.value("name").toString().toHtmlEscaped();
            quest["score"] = record.value("score").toInt();
            quest["subject"] = record.value("subject").toString();
            quest["count_user_solved"] = record.value("count_user_solved").toInt();
            answer["quest"] = quest;

            QJsonObject user; // TODO deprecated
            user["id"] = record.value("userid").toInt();
            user["logo"] = record.value("logo").toString();
            user["nick"] = record.value("nick").toString().toHtmlEscaped();
            user["email"] = record.value("email").toString().toHtmlEscaped();
            answer["user"] = user;

            answerlist.push_back(answer);
        }
    }

    jsonResponse["data"] = answerlist;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// *********** Quest Hint Delete ************
// *******************************************


CmdHandlerDeleteHint::CmdHandlerDeleteHint()
    : CmdHandlerBase("deletehint", "Method for delete hint from quest"){
    TAG = "CmdHandlerDeleteHint";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("hintid").required().integer_().description("hint id"));
}

// ---------------------------------------------------------------------

void CmdHandlerDeleteHint::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int hintid = jsonRequest["hintid"].toInt();
    if(hintid == 0){
        pRequest->sendMessageError(cmd(), Errors::HintIDMustBeNotZero()); // TODO redesign
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

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest id"));
}

// ---------------------------------------------------------------------

void CmdHandlerHints::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int questid = jsonRequest["questid"].toInt();
    if(questid == 0){
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
        return;
    }

    QJsonArray hints;
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
        QJsonObject hint;
        hint["id"] = hintid;
        hint["questid"] = questid2;
        hint["text"] = text;
        hint["dt"] = dt;
        hints.push_back(hint);
    }

    jsonResponse["data"] = hints;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// ********* Quests Proposal List ************
// *******************************************


CmdHandlerQuestsProposalList::CmdHandlerQuestsProposalList()
    : CmdHandlerBase("quests_proposal_list", "Quests proposal list"){
    TAG = "CmdHandlerQuestsProposalList";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("onpage").integer_().optional().description("On page"));
    m_vInputs.push_back(CmdInputDef("page").integer_().optional().description("page"));
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

// *******************************************
// ************ Quests WriteUps **************
// *******************************************


CmdHandlerWriteUps::CmdHandlerWriteUps()
    : CmdHandlerBase("writeups", "This method will be returned list of writeups by questid"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest ID"));
}

// ---------------------------------------------------------------------

void CmdHandlerWriteUps::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    // bool bConvert = false;

    int questid = jsonRequest["questid"].toInt();
    if(questid == 0){
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
        return;
    }

    QJsonArray writeups;
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM quests_writeups WHERE questid = :questid");
    query.bindValue(":questid", questid);
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        int hintid = record.value("id").toInt();
        int questid2 = record.value("questid").toInt();
        QString type = record.value("type").toString();
        QString link = record.value("link").toString();
        QString text = record.value("text").toString();
        QString dt = record.value("dt").toString();
        QJsonObject writeup;
        writeup["writeupid"] = hintid;
        writeup["questid"] = questid2;
        writeup["type"] = type;
        writeup["link"] = link;
        writeup["text"] = text;
        writeup["dt"] = dt;
        writeups.push_back(writeup);
    }

    jsonResponse["data"] = writeups;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
