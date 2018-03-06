#include <cmd_server_settings_handler.h>
#include <QJsonArray>
// #include <memory_cache_serversettings.h>
#include <employ_settings.h>

CmdServerSettingsHandler::CmdServerSettingsHandler(){
}

std::string CmdServerSettingsHandler::cmd(){
        return "server_settings";
}

bool CmdServerSettingsHandler::accessUnauthorized(){
	return false;
}

bool CmdServerSettingsHandler::accessUser(){
	return false;
}

bool CmdServerSettingsHandler::accessTester(){
	return false;
}

bool CmdServerSettingsHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdServerSettingsHandler::inputs(){
	return m_vInputs;
};

QString CmdServerSettingsHandler::description(){
        return "Return server settings";
}

QStringList CmdServerSettingsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdServerSettingsHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
	
    EmploySettings *pSettings = findEmploy<EmploySettings>();

    jsonResponse["data"] = pSettings->toJsonArray(); // TODO how much db connections and time
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
