#include <cmd_handler_mail_send.h>
#include <SmtpMime>
#include <employ_settings.h>

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

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString sEmail = jsonRequest["to"].toString();

    QString sSubject = jsonRequest["subject"].toString();
    QString sBody = jsonRequest["body"].toString();

    QString sMailHost = pSettings->getSettString("mail_host");
    int nMailPort = pSettings->getSettInteger("mail_port");
    QString sMailPassword = pSettings->getSettPassword("mail_password");
    QString sMailFrom = pSettings->getSettString("mail_from");

    SmtpClient smtp(sMailHost, nMailPort, SmtpClient::SslConnection);
    smtp.setUser(sMailFrom);
    smtp.setPassword(sMailPassword);

    MimeMessage message;

    EmailAddress sender(sMailFrom, "FreeHackQuest");
    message.setSender(&sender);

    EmailAddress to(sEmail, "");
    message.addRecipient(&to);

    message.setSubject(sSubject);

    MimeText text;
    text.setText(sBody);

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
