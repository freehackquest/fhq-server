#include <cmd_update_server_settings_handler.h>
#include <QJsonArray>
#include <memory_cache_serversettings.h>

CmdUpdateServerSettingsHandler::CmdUpdateServerSettingsHandler(){
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("name of setting"));
    m_vInputs.push_back(CmdInputDef("value").required().any_().description("value of setting"));
}

QString CmdUpdateServerSettingsHandler::cmd(){
        return "update_server_settings";
}

bool CmdUpdateServerSettingsHandler::accessUnauthorized(){
	return false;
}

bool CmdUpdateServerSettingsHandler::accessUser(){
	return false;
}

bool CmdUpdateServerSettingsHandler::accessTester(){
	return false;
}

bool CmdUpdateServerSettingsHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdUpdateServerSettingsHandler::inputs(){
	return m_vInputs;
};

QString CmdUpdateServerSettingsHandler::description(){
        return "Return server settings";
}

QStringList CmdUpdateServerSettingsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUpdateServerSettingsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serversettings");
	if(pMemoryCache == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
		return;
	}

    // ServerSettHelper


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
