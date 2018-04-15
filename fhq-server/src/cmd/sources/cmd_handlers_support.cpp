#include <cmd_handlers_support.h>
#include <log.h>
#include <runtasks.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <SmtpMime>

// ******************************
// * This handler for add support
// ******************************

CmdHandlerFeedbackAdd::CmdHandlerFeedbackAdd()
    : CmdHandlerBase("feedback_add", "Create the feedback"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("from").email_().required().description("From user"));
    m_vInputs.push_back(CmdInputDef("text").string_().required().description("Text of feedback"));
    m_vInputs.push_back(CmdInputDef("type").string_().required().description("Type"));
}

// ---------------------------------------------------------------------

void CmdHandlerFeedbackAdd::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int nUserID = 0;
    QString sEmail = jsonRequest["from"].toString().trimmed();
    QString sText = jsonRequest["text"].toString().trimmed();
    QString sType = jsonRequest["type"].toString().trimmed();

    IUserToken *pUserToken = pRequest->userToken();
    if(pUserToken != NULL){
        sEmail = pUserToken->email();
        nUserID = pUserToken->userid();
    }
    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QSqlQuery query(db);
    query.prepare("INSERT INTO feedback(`type`, `from`, `text`, `userid`, `dt`) VALUES(:type,:from,:text,:userid,NOW());");
    query.bindValue(":type", sType);
    query.bindValue(":from", sEmail);
    query.bindValue(":text", sText);
    query.bindValue(":userid", nUserID);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    RunTasks::AddPublicEvents("users", "Added feedback");

    QString sMailToAdmin = pSettings->getSettString("mail_system_message_admin_email");
    QString sSubject = "Feedback (FreeHackQuest 2017)";
    QString sContext = "Feedback \n"
                       "Type: " + sType + "\n"
                       "From: " + sEmail + "\n"
                       "Text: \n" + sText + "\n";

    RunTasks::MailSend(pRequest->server(), sMailToAdmin, sSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
