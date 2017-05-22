#include "../headers/cmd_serverinfo_handler.h"
#include <QJsonArray>
#include <memory_cache_serverinfo.h>

QString CmdServerInfoHandler::cmd(){
	return "serverinfo";
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

QString CmdServerInfoHandler::short_description(){
	return "Return server information";
}

QString CmdServerInfoHandler::description(){
	return "Return server information";
}

QStringList CmdServerInfoHandler::errors(){
	QStringList	list;
	return list;
}

void CmdServerInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	
	IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serverinfo");
	if(pMemoryCache == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::InternalServerError());
		return;
	}

	MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);
	jsonData["data"] = pMemoryCacheServerInfo->toJsonObject();
	jsonData["result"] = QJsonValue("DONE");
	pWebSocketServer->sendMessage(pClient, jsonData);
}
