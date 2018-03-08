#include <cmd_mails_list_handler.h>
#include <QJsonArray>
#include <SmtpMime>
// #include <memory_cache_serverinfo.h>
#include <employ_settings.h>

CmdMailsListHandler::CmdMailsListHandler(){
    TAG = "CmdMailsListHandler";
    // Log::info(TAG, "Created");

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

std::string CmdMailsListHandler::cmd(){
        return "mails_list";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdMailsListHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdMailsListHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdMailsListHandler::description(){
        return "Mails list";
}

// ---------------------------------------------------------------------

void CmdMailsListHandler::handle(ModelRequest *pRequest){
    // QJsonObject jsonRequest = pRequest->data();
    // QJsonObject jsonResponse;

    pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
}
