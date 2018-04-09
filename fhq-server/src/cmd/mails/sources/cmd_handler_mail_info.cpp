#include <cmd_handler_mail_info.h>
#include <QJsonArray>
#include <SmtpMime>
// #include <memory_cache_serverinfo.h>
#include <employ_settings.h>
#include <employ_database.h>

CmdHandlerMailInfo::CmdHandlerMailInfo(){
    TAG = "CmdHandlerMailInfo";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

std::string CmdHandlerMailInfo::cmd(){
        return "mail_info";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerMailInfo::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerMailInfo::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerMailInfo::description(){
        return "Mails list";
}

// ---------------------------------------------------------------------

void CmdHandlerMailInfo::handle(ModelRequest *pRequest){
    // QJsonObject jsonRequest = pRequest->data();
    // QJsonObject jsonResponse;

    pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
}
