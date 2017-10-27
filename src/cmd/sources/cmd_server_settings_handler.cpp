#include <cmd_server_settings_handler.h>
#include <QJsonArray>
#include <memory_cache_serversettings.h>

CmdServerSettingsHandler::CmdServerSettingsHandler(){
}

QString CmdServerSettingsHandler::cmd(){
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

const QVector<CmdInputDef> &CmdServerSettingsHandler::inputs(){
	return m_vInputs;
};

QString CmdServerSettingsHandler::description(){
        return "Return server settings";
}

QStringList CmdServerSettingsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdServerSettingsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject /*obj*/){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	
    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serversettings");
	if(pMemoryCache == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
		return;
	}

    MemoryCacheServerSettings *pMemoryCacheServerSettings = dynamic_cast<MemoryCacheServerSettings*>(pMemoryCache);
    jsonData["data"] = pMemoryCacheServerSettings->toJsonArray(); // TODO how much db connections and time
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
