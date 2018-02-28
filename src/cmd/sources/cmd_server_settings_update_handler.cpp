#include <cmd_server_settings_update_handler.h>
#include <QJsonArray>
#include <memory_cache_serversettings.h>

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

void CmdServerSettingsUpdateHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));

    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serversettings");
	if(pMemoryCache == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
		return;
	}

    QString sName = obj["name"].toString();
    QString sNewValue = obj["value"].toString();

    MemoryCacheServerSettings *pMemoryCacheServerSettings = dynamic_cast<MemoryCacheServerSettings*>(pMemoryCache);

    if(!pMemoryCacheServerSettings->hasSett(sName)){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("Setting with name: " + sName + " did not found"));
        return;
    }

    QString sType = pMemoryCacheServerSettings->getSettType(sName);

    if(sType == SETT_TYPE_STRING){
        pMemoryCacheServerSettings->setSettString(sName, sNewValue);
    }else if(sType == SETT_TYPE_PASSWORD){
        pMemoryCacheServerSettings->setSettPassword(sName, sNewValue);
    }else if(sType == SETT_TYPE_INTEGER){
        pMemoryCacheServerSettings->setSettInteger(sName, sNewValue.toInt());
    }else if(sType == SETT_TYPE_BOOLEAN){
        pMemoryCacheServerSettings->setSettBoolean(sName, sNewValue == "yes");
    }else{
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotImplementedYet());
        return;
    }

	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
