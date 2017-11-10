#include <cmd_server_info_handler.h>
#include <QJsonArray>
#include <log.h>
#include <iostream>
#include <memory_cache_serverinfo.h>

CmdServerInfoHandler::CmdServerInfoHandler(){
    TAG = "CmdServerInfoHandler";
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
    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());
    jsonResponse["m"] = QJsonValue(m);
    QJsonObject jsonData;

    QJsonObject jsonRequestStatistics;
	IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serverinfo");
	if(pMemoryCache == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
		return;
	}

	MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);
    jsonRequestStatistics = pMemoryCacheServerInfo->toJsonObject(); // TODO how much db connections and time

    jsonData["request_statistics"] = jsonRequestStatistics;
    jsonData["server_started"] = pMemoryCacheServerInfo->getServerStart().toString(Qt::ISODate);
    qint64 updatime = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    updatime = updatime - pMemoryCacheServerInfo->getServerStart().toSecsSinceEpoch();
    jsonData["server_uptime_sec"] = updatime;

    /* NOT WORK
     * QJsonArray lastLogMessages;
    {
        QMutexLocker locker(&g_LOG_MUTEX);
        int len = g_LAST_LOG_MESSAGES.size();
        g_LAST_LOG_MESSAGES << "log";
        for(int i = 0; i < len; i++){
            lastLogMessages.append(g_LAST_LOG_MESSAGES[i]);
        }
    }
    jsonData["last_log_messages"] = lastLogMessages;
    */
    jsonData["last_log_messages"] = Log::last_logs();

    jsonResponse["data"] = jsonData;
    jsonResponse["result"] = QJsonValue("DONE");
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}
