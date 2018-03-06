#include <cmd_server_settings_update_handler.h>
#include <QJsonArray>
#include <employ_settings.h>

CmdServerSettingsUpdateHandler::CmdServerSettingsUpdateHandler(){
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("name of setting"));
    m_vInputs.push_back(CmdInputDef("value").required().any_().description("value of setting"));
}

std::string CmdServerSettingsUpdateHandler::cmd(){
        return "server_settings_update";
}

bool CmdServerSettingsUpdateHandler::accessUnauthorized(){
	return false;
}

bool CmdServerSettingsUpdateHandler::accessUser(){
	return false;
}

bool CmdServerSettingsUpdateHandler::accessTester(){
	return false;
}

bool CmdServerSettingsUpdateHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdServerSettingsUpdateHandler::inputs(){
	return m_vInputs;
};

QString CmdServerSettingsUpdateHandler::description(){
        return "Update server settings";
}

QStringList CmdServerSettingsUpdateHandler::errors(){
	QStringList	list;
	return list;
}

void CmdServerSettingsUpdateHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString sName = jsonRequest["name"].toString();
    QString sNewValue = jsonRequest["value"].toString();

    if(!pSettings->hasSett(sName)){
        pRequest->sendMessageError(cmd(), Errors::NotFound("Setting with name: " + sName + " did not found"));
        return;
    }

    QString sType = pSettings->getSettType(sName);

    if(sType == SETT_TYPE_STRING){
        pSettings->setSettString(sName, sNewValue);
    }else if(sType == SETT_TYPE_PASSWORD){
        pSettings->setSettPassword(sName, sNewValue);
    }else if(sType == SETT_TYPE_INTEGER){
        pSettings->setSettInteger(sName, sNewValue.toInt());
    }else if(sType == SETT_TYPE_BOOLEAN){
        pSettings->setSettBoolean(sName, sNewValue == "yes");
    }else{
        pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
