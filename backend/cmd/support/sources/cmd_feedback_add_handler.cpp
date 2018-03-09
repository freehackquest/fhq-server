#include <cmd_feedback_add_handler.h>
#include <runtasks.h>
#include <QRegularExpression>
#include <employ_settings.h>
#include <SmtpMime>
#include <QJsonArray>
#include <QCryptographicHash>

CmdFeedbackAddHandler::CmdFeedbackAddHandler(){
    TAG = "CmdFeedbackAddHandler";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("from").string_().required().description("From user"));
    m_vInputs.push_back(CmdInputDef("text").string_().required().description("Text of feedback"));
    m_vInputs.push_back(CmdInputDef("type").string_().required().description("Type"));
}

// ---------------------------------------------------------------------

std::string CmdFeedbackAddHandler::cmd(){
    return "feedback_add";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdFeedbackAddHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdFeedbackAddHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdFeedbackAddHandler::description(){
	return "Create the quest";
}

// ---------------------------------------------------------------------

void CmdFeedbackAddHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

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

    QRegularExpression regexEmail("^[0-9a-zA-Z]{1}[0-9a-zA-Z-._]*[0-9a-zA-Z]{1}@[0-9a-zA-Z]{1}[-.0-9a-zA-Z]*[0-9a-zA-Z]{1}\\.[a-zA-Z]{2,6}$");
    if(!regexEmail.match(sEmail).hasMatch()){
        Log::err(TAG, "Invalid email format " + sEmail);
        pRequest->sendMessageError(cmd(), Error(400, "Expected email format"));
        return;
    }

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

    RunTasks::AddPublicEvents(pRequest->server(), "users", "Added feedback");
	
    // TODO move to tasks
    QString sMailHost = pSettings->getSettString("mail_host");
    int nMailPort = pSettings->getSettInteger("mail_port");
    QString sMailPassword = pSettings->getSettPassword("mail_password");
    QString sMailFrom = pSettings->getSettString("mail_from");
    QString sMailToAdmin = pSettings->getSettString("mail_system_message_admin_email");

    SmtpClient smtp(sMailHost, nMailPort, SmtpClient::SslConnection);
    smtp.setUser(sMailFrom);
    smtp.setPassword(sMailPassword);

    MimeMessage message;

    EmailAddress sender(sMailFrom, "FreeHackQuest");
    message.setSender(&sender);

    EmailAddress to(sMailToAdmin, "");
    message.addRecipient(&to);

    message.setSubject("Feedback (FreeHackQuest 2017)");
    MimeText text;
    text.setText("Feedback \n"
                 "Type: " + sType + "\n"
                 "From: " + sEmail + "\n"
                 "Text: \n" + sText + "\n"
              );

    message.addPart(&text);

    // Now we can send the mail
    if (!smtp.connectToHost()) {
        pRequest->sendMessageError(cmd(), Error(500, "[MAIL] Failed to connect to host!"));
        return;
    }

    if (!smtp.login()) {
        pRequest->sendMessageError(cmd(), Error(500, "[MAIL] Failed to login!"));
        return;
    }

    if (!smtp.sendMail(message)) {
        pRequest->sendMessageError(cmd(), Error(500, "[MAIL] Failed to send mail!"));
        return;
    }
    smtp.quit();

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
