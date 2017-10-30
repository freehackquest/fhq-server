#include <cmd_server_info_handler.h>
#include <QJsonArray>
#include <memory_cache_serverinfo.h>

CmdServerInfoHandler::CmdServerInfoHandler(){
}

QString CmdServerInfoHandler::cmd(){
    return "server_info";
}

bool CmdServerInfoHandler::accessUnauthorized(){
	return false;
}

bool CmdServerInfoHandler::accessUser(){
	return false;
}

bool CmdServerInfoHandler::accessTester(){
	return false;
}

bool CmdServerInfoHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdServerInfoHandler::inputs(){
	return m_vInputs;
};

QString CmdServerInfoHandler::description(){
	return "Return server information";
}

QStringList CmdServerInfoHandler::errors(){
	QStringList	list;
	return list;
}

void CmdServerInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject /*obj*/){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	
	IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serverinfo");
	if(pMemoryCache == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
		return;
	}

	MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);
	jsonData["data"] = pMemoryCacheServerInfo->toJsonObject(); // TODO how much db connections and time
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
