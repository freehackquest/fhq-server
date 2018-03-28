#include <cmd_handler_mail_send.h>
#include <SmtpMime>
#include <runtasks.h>

CmdHandlerMailSend::CmdHandlerMailSend(){
    TAG = "CmdHandlerMailSend";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("to").required().email_().description("E-mail of the recipient"));
    m_vInputs.push_back(CmdInputDef("subject").required().string_().description("Subject of the message"));
    m_vInputs.push_back(CmdInputDef("body").required().string_().description("Body of the message"));

}

// ---------------------------------------------------------------------

std::string CmdHandlerMailSend::cmd(){
        return "mail_send";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerMailSend::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerMailSend::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerMailSend::description(){
        return "Mails list";
}

// ---------------------------------------------------------------------

void CmdHandlerMailSend::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sEmail = jsonRequest["to"].toString();
    QString sSubject = jsonRequest["subject"].toString();
    QString sBody = jsonRequest["body"].toString();

    RunTasks::MailSend(pRequest->server(), sEmail, sSubject, sBody);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
