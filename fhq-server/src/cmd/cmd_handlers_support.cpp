#include <cmd_handlers_support.h>
#include <runtasks.h>
#include <iostream>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <QSqlQuery> // TODO deprecated
#include <QSqlRecord> // TODO deprecated
#include <QString> // TODO deprecated
#include <QVariant> // TODO deprecated

// ******************************
// * This handler for add support
// ******************************

CmdHandlerFeedbackAdd::CmdHandlerFeedbackAdd()
    : WsjcppJsonRpc20HandlerBase("feedback_add", "Create the feedback") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("from", "From user")
        .addValidator(new WsjcppValidatorEmail());
    requireStringParam("text", "Text of feedback");
    requireStringParam("type", "Type of feedback"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerFeedbackAdd::handle(WsjcppJsonRpc20Request *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int nUserID = 0;
    std::string sEmail = pRequest->getInputString("from", "");
    sEmail = WsjcppCore::trim(sEmail);
    std::string sText = pRequest->getInputString("text", "");
    sText = WsjcppCore::trim(sText);
    std::string sType = pRequest->getInputString("type", "");
    sType = WsjcppCore::trim(sType);

    WsjcppJsonRpc20UserSession *pUserSession = pRequest->getUserSession();
    if (pUserSession != NULL) {
        sEmail = pUserSession->getUserEmail();
        nUserID = pUserSession->getUserId();
    }
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

    QSqlQuery query(db);
    query.prepare("INSERT INTO feedback(`type`, `from`, `text`, `userid`, `dt`) VALUES(:type,:from,:text,:userid,NOW());");
    query.bindValue(":type", QString::fromStdString(sType));
    query.bindValue(":from", QString::fromStdString(sEmail));
    query.bindValue(":text", QString::fromStdString(sText));
    query.bindValue(":userid", nUserID);
    if (!query.exec()) {
        pRequest->fail(WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonMeta;
    RunTasks::AddPublicEvents("users", "Added feedback", jsonMeta);

    // TODO move to EmployMails
    std::string sMailToAdmin = pGlobalSettings->get("mail_system_message_admin_email").getStringValue();
    std::string sSubject = "Feedback (FreeHackQuest 2017)";
    std::string sContext = "Feedback \n"
                       "Type: " + sType + "\n"
                       "From: " + sEmail + "\n"
                       "Text: \n" + sText + "\n";

    RunTasks::MailSend(sMailToAdmin, sSubject, sContext);

    pRequest->done(jsonResponse);
}
