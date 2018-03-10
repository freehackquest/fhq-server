#include <cmd_handler_mail_send.h>
#include <QJsonArray>
#include <SmtpMime>
// #include <memory_cache_serverinfo.h>
#include <employ_settings.h>

CmdHandlerMailSend::CmdHandlerMailSend(){
    TAG = "CmdHandlerMailSend";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
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
    // QJsonObject jsonRequest = pRequest->data();
    // QJsonObject jsonResponse;

    pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
}
