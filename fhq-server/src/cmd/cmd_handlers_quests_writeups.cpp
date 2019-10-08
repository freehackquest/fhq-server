#include <cmd_handlers_quests_writeups.h>
#include <runtasks.h>
#include <fallen.h>
#include <md5.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <employ_notify.h>

REGISTRY_CMD(CmdHandlerQuestsWriteUpsList)

CmdHandlerQuestsWriteUpsList::CmdHandlerQuestsWriteUpsList()
    : CmdHandlerBase("quests_writeups_list", "This method will be returned list of writeups by questid") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);
    setActivatedFromVersion("0.2.18");

    requireIntegerParam("questid", "Quest ID");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsWriteUpsList::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();

    // get quest id
    int nQuestID = 0;
    if (jsonRequest["questid"].is_number_integer()) {
        nQuestID = jsonRequest.at("questid");
    }

    if (nQuestID == 0) {
        pRequest->sendMessageError(cmd(), WSJCppError(400, "'questid' must be none zero"));
        return;
    }


    // user session
    WSJCppUserSession *pUserSession = pRequest->getUserSession();
    bool bAdmin = pRequest->isAdmin();
    int nUserID = 0;
    if (pUserSession != nullptr) { // TODO refactor to pRequest->userId()
        nUserID = pUserSession->userid();
    }

    std::string sSQLRequest = ""
        " SELECT qw.id, qw.questid, qw.type, qw.link, qw.userid, qw.approve, qw.dt, u.nick, u.logo FROM quests_writeups qw "
        " LEFT JOIN users u ON u.id = qw.userid "
        " WHERE qw.questid = :questid ";
    if (!bAdmin) {
        sSQLRequest += " AND (qw.approve = :approve OR qw.userid = :userid)";
    }

    nlohmann::json jsonResponse;
    nlohmann::json jsonWriteups = nlohmann::json::array();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare(QString::fromStdString(sSQLRequest));
    query.bindValue(":questid", nQuestID);
    if (!bAdmin) {
        query.bindValue(":approve", 1);
        query.bindValue(":userid", nUserID);
    }
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonWriteup;
        jsonWriteup["writeupid"] = record.value("id").toInt();
        jsonWriteup["questid"] = record.value("questid").toInt();
        jsonWriteup["userid"] = record.value("userid").toInt();
        jsonWriteup["type"] = record.value("type").toString().toStdString();
        jsonWriteup["link"] = record.value("link").toString().toStdString();
        jsonWriteup["dt"] = record.value("dt").toString().toStdString();
        jsonWriteup["user_nick"] = record.value("nick").toString().toStdString();
        jsonWriteup["user_logo"] = record.value("logo").toString().toStdString();
        jsonWriteup["approve"] = record.value("approve").toInt();
        jsonWriteups.push_back(jsonWriteup);
    }

    jsonResponse["data"] = jsonWriteups;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Quests WriteUps Proposal

REGISTRY_CMD(CmdHandlerQuestsWriteUpsProposal)

CmdHandlerQuestsWriteUpsProposal::CmdHandlerQuestsWriteUpsProposal()
    : CmdHandlerBase("quests_writeups_proposal", "This method will be add writeup from authorized user") {

    m_sLinkPrefix = "https://www.youtube.com/watch?v=";

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);
    setActivatedFromVersion("0.2.18");

    // validation and description input fields
    requireIntegerParam("questid", "Quest ID");
    requireStringParam("writeup_link", "Link to writeup (Must be start from " + m_sLinkPrefix + ")"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsWriteUpsProposal::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();

    // get quest id
    int nQuestID = 0;
    if (jsonRequest["questid"].is_number_integer()) {
        nQuestID = jsonRequest.at("questid");
    }

    if (nQuestID == 0) {
        pRequest->sendMessageError(cmd(), WSJCppError(400, "'questid' must be none zero"));
        return;
    }

    // writeup_link
    std::string sWriteUpLink = "";
    if (jsonRequest["writeup_link"].is_string()) {
        sWriteUpLink = jsonRequest.at("writeup_link");
    }

    if (sWriteUpLink.rfind(m_sLinkPrefix, 0) != 0) {
        pRequest->sendMessageError(cmd(), WSJCppError(400, "Expected link starts from '" + m_sLinkPrefix + "'"));
        return;
    }

    sWriteUpLink = "https://www.youtube.com/embed/" + sWriteUpLink.substr(m_sLinkPrefix.length());

    // user token
    WSJCppUserSession *pUserSession = pRequest->getUserSession();
    bool bAdmin = pRequest->isAdmin();
    int nUserID = 0;
    QString sUserEmail = "";
    if (pUserSession != nullptr) {
        nUserID = pUserSession->userid();
        sUserEmail = pUserSession->email();
    }

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO quests_writeups(questid,type,link,text,dt,approve,userid) VALUES(:questid, 'youtube_video', :writeup_link, '', NOW(), :approve, :userid)");
    query.bindValue(":questid", nQuestID);
    query.bindValue(":writeup_link", QString::fromStdString(sWriteUpLink));
    query.bindValue(":approve", bAdmin ? 1 : 0);
    query.bindValue(":userid", nUserID);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WSJCppError(500, query.lastError().text().toStdString()));
        return;
    }
    int nWriteUpID = query.lastInsertId().toInt();
    if (bAdmin) {
        EmployNotify *pNotify = findEmploy<EmployNotify>();
        ModelNotification notification("info", "quests", "Added [writeup#" + std::to_string(nWriteUpID) + "] for [quest#" + std::to_string(nQuestID) + "]");
        pNotify->sendNotification(notification);
    } else {
        EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();
        std::string sMailToAdmin = pGlobalSettings->get("mail_system_message_admin_email").getStringValue();
        std::string sMessageSubject = "Quest WriteUp Proposal (FreeHackQuest)";
        std::string sContext = "Quest WriteUp Proposal\n"
                        "UserID: #" + std::to_string(nUserID) + "\n"
                        "From: " + sUserEmail.toStdString() + "\n"
                        "QuestId: #" + std::to_string(nQuestID) + "\n";
                        "WriteUpID: #" + std::to_string(nWriteUpID) + "\n";

        RunTasks::MailSend(sMailToAdmin, sMessageSubject, sContext);
    }

    nlohmann::json jsonResponse;
    nlohmann::json jsonData;
    jsonData["writeupid"] = nWriteUpID;
    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);

    // TODO send mail to admin if it was not admin
}

// ---------------------------------------------------------------------
// Quests WriteUps Update

REGISTRY_CMD(CmdHandlerQuestsWriteUpsUpdate)

CmdHandlerQuestsWriteUpsUpdate::CmdHandlerQuestsWriteUpsUpdate()
    : CmdHandlerBase("quests_writeups_update", "This method will be update writeup approve now") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);
    setActivatedFromVersion("0.2.18");

    // validation and description input fields
    requireIntegerParam("writeupid", "WriteUp ID");
    requireStringParam("approve", "Approve value 1 or 0"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsWriteUpsUpdate::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();

    // get quest id
    int nWriteUpID = 0;
    if (jsonRequest["writeupid"].is_number_integer()) {
        nWriteUpID = jsonRequest.at("writeupid");
    }

    // get quest id
    int nApprove = 0;
    if (jsonRequest["approve"].is_number_integer()) {
        nApprove = jsonRequest.at("approve");
    }

    nlohmann::json jsonWriteUp;
    int nQuestIDValue = 0;
    int nCurrentApproveValue = 0;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM quests_writeups WHERE id = :writeupid");
    query.bindValue(":writeupid", nWriteUpID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WSJCppError(500, query.lastError().text().toStdString()));
        return;
    }
    
    if (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonWriteup;
        nQuestIDValue = record.value("questid").toInt();
        jsonWriteUp["questid"] = nQuestIDValue;
        nCurrentApproveValue = record.value("approve").toInt();
    } else {
        pRequest->sendMessageError(cmd(), WSJCppError(404, "Not found writeup"));
        return;
    }

    if (nCurrentApproveValue != nApprove) {
        QSqlQuery query2(db);
        query2.prepare("UPDATE quests_writeups SET approve = :approve WHERE id = :writeupid");
        query2.bindValue(":approve", nApprove);
        query2.bindValue(":writeupid", nWriteUpID);
        if (!query2.exec()) {
            pRequest->sendMessageError(cmd(), WSJCppError(500, query2.lastError().text().toStdString()));
            return;
        }
        if (nApprove == 1) {
            EmployNotify *pNotify = findEmploy<EmployNotify>();
            ModelNotification notification("info", "quests", "Approved [writeup#" + std::to_string(nWriteUpID) + "] for [quest#" + std::to_string(nQuestIDValue) + "]");
            pNotify->sendNotification(notification);
        }
    }
   
    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonWriteUp;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


// ---------------------------------------------------------------------
// Quests WriteUps Delete

REGISTRY_CMD(CmdHandlerQuestsWriteUpsDelete)

CmdHandlerQuestsWriteUpsDelete::CmdHandlerQuestsWriteUpsDelete()
    : CmdHandlerBase("quests_writeups_delete", "This method will be remove writeup") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);
    setActivatedFromVersion("0.2.18");

    // validation and description input fields
    requireIntegerParam("writeupid", "WriteUp ID");
}

// ---------------------------------------------------------------------

void CmdHandlerQuestsWriteUpsDelete::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();

    // get quest id
    int nWriteUpID = 0;
    if (jsonRequest["writeupid"].is_number_integer()) {
        nWriteUpID = jsonRequest.at("writeupid");
    }

    nlohmann::json jsonWriteUp;
    int nQuestIDValue = 0;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM quests_writeups WHERE id = :writeupid");
    query.bindValue(":writeupid", nWriteUpID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WSJCppError(500, query.lastError().text().toStdString()));
        return;
    }
    
    if (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonWriteup;
        nQuestIDValue = record.value("questid").toInt();
    } else {
        pRequest->sendMessageError(cmd(), WSJCppError(404, "Not found writeup"));
        return;
    }

    
    QSqlQuery query2(db);
    query2.prepare("DELETE FROM quests_writeups WHERE id = :writeupid");
    query2.bindValue(":writeupid", nWriteUpID);
    if (!query2.exec()) {
        pRequest->sendMessageError(cmd(), WSJCppError(500, query2.lastError().text().toStdString()));
        return;
    }
    
    EmployNotify *pNotify = findEmploy<EmployNotify>();
    ModelNotification notification("warning", "quests", "Removed [writeup#" + std::to_string(nWriteUpID) + "] for [quest#" + std::to_string(nQuestIDValue) + "]");
    pNotify->sendNotification(notification);
       
    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonWriteUp;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
