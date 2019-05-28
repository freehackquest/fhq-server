#include <cmd_handlers_support.h>
#include <fallen.h>
#include <runtasks.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>

// ******************************
// * This handler for add support
// ******************************

CmdHandlerFeedbackAdd::CmdHandlerFeedbackAdd()
    : CmdHandlerBase("feedback_add", "Create the feedback") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("from", "From user")
        .addValidator(new ValidatorEmail());
    requireStringParam("text", "Text of feedback");
    requireStringParam("type", "Type of feedback"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerFeedbackAdd::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int nUserID = 0;
    QString sEmail = jsonRequest["from"].toString().trimmed();
    QString sText = jsonRequest["text"].toString().trimmed();
    QString sType = jsonRequest["type"].toString().trimmed();

    WSJCppUserSession *pUserSession = pRequest->userSession();
    if (pUserSession != NULL) {
        sEmail = pUserSession->email();
        nUserID = pUserSession->userid();
    }
    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QSqlQuery query(db);
    query.prepare("INSERT INTO feedback(`type`, `from`, `text`, `userid`, `dt`) VALUES(:type,:from,:text,:userid,NOW());");
    query.bindValue(":type", sType);
    query.bindValue(":from", sEmail);
    query.bindValue(":text", sText);
    query.bindValue(":userid", nUserID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WSJCppError(500, query.lastError().text().toStdString()));
        return;
    }

    RunTasks::AddPublicEvents("users", "Added feedback");

    std::string sMailToAdmin = pSettings->getSettString("mail_system_message_admin_email").toStdString();
    std::string sSubject = "Feedback (FreeHackQuest 2017)";
    std::string sContext = "Feedback \n"
                       "Type: " + sType.toStdString() + "\n"
                       "From: " + sEmail.toStdString() + "\n"
                       "Text: \n" + sText.toStdString() + "\n";

    RunTasks::MailSend(sMailToAdmin, sSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
