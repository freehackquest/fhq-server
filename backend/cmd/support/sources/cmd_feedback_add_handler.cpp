#include <cmd_feedback_add_handler.h>
#include <runtasks.h>
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
    m_vInputs.push_back(CmdInputDef("from").email_().required().description("From user"));
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
	
    QString sMailToAdmin = pSettings->getSettString("mail_system_message_admin_email");
    QString sSubject = "Feedback (FreeHackQuest 2017)";
    QString sContext = "Feedback \n"
                       "Type: " + sType + "\n"
                       "From: " + sEmail + "\n"
                       "Text: \n" + sText + "\n";

    RunTasks::MailSend(pRequest->server(), sMailToAdmin, sSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
