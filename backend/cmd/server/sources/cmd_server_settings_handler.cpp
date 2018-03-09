#include <cmd_server_settings_handler.h>
#include <QJsonArray>
// #include <memory_cache_serversettings.h>
#include <employ_settings.h>

CmdServerSettingsHandler::CmdServerSettingsHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

std::string CmdServerSettingsHandler::cmd(){
        return "server_settings";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdServerSettingsHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdServerSettingsHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdServerSettingsHandler::description(){
        return "Return server settings";
}

// ---------------------------------------------------------------------

void CmdServerSettingsHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
	
    EmploySettings *pSettings = findEmploy<EmploySettings>();

    jsonResponse["data"] = pSettings->toJsonArray(); // TODO how much db connections and time
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
