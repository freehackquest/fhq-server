#include <cmd_server_info_handler.h>
#include <QJsonArray>
#include <log.h>
#include <iostream>
#include <memory_cache_serverinfo.h>

CmdServerInfoHandler::CmdServerInfoHandler(){
    TAG = "CmdServerInfoHandler";
}

std::string CmdServerInfoHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdServerInfoHandler::inputs(){
	return m_vInputs;
};

QString CmdServerInfoHandler::description(){
	return "Return server information";
}

QStringList CmdServerInfoHandler::errors(){
	QStringList	list;
	return list;
}

void CmdServerInfoHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QJsonObject jsonRequestStatistics;
    IMemoryCache *pMemoryCache = pRequest->server()->findMemoryCache("serverinfo");
	if(pMemoryCache == NULL){
        pRequest->sendMessageError(cmd(), Errors::InternalServerError());
		return;
	}

	MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);
    jsonRequestStatistics = pMemoryCacheServerInfo->toJsonObject(); // TODO how much db connections and time

    jsonResponse["request_statistics"] = jsonRequestStatistics;
    jsonResponse["server_started"] = pMemoryCacheServerInfo->getServerStart().toString(Qt::ISODate);
    qint64 updatime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    updatime = updatime - pMemoryCacheServerInfo->getServerStart().toMSecsSinceEpoch();
    jsonResponse["server_uptime_sec"] = updatime/1000;

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
    jsonResponse["last_log_messages"] = Log::last_logs();
    // jsonResponse["data"] = jsonData;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
