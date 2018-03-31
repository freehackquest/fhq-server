#include <cmd_server_info_handler.h>
#include <QJsonArray>
#include <log.h>
#include <iostream>
#include <memory_cache_serverinfo.h>

CmdServerInfoHandler::CmdServerInfoHandler(){
    TAG = "CmdServerInfoHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

std::string CmdServerInfoHandler::cmd(){
    return "server_info";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdServerInfoHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdServerInfoHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdServerInfoHandler::description(){
	return "Return server information";
}

// ---------------------------------------------------------------------

void CmdServerInfoHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
    QJsonObject data;

    QJsonObject jsonRequestStatistics;
    IMemoryCache *pMemoryCache = pRequest->server()->findMemoryCache("serverinfo");
	if(pMemoryCache == NULL){
        pRequest->sendMessageError(cmd(), Errors::InternalServerError());
		return;
	}

	MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);
    jsonRequestStatistics = pMemoryCacheServerInfo->toJsonObject(); // TODO how much db connections and time

    data["request_statistics"] = jsonRequestStatistics;
    data["server_started"] = pMemoryCacheServerInfo->getServerStart().toString(Qt::ISODate);
    qint64 updatime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    updatime = updatime - pMemoryCacheServerInfo->getServerStart().toMSecsSinceEpoch();
    data["server_uptime_sec"] = updatime/1000;

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
    data["last_log_messages"] = lastLogMessages;
    */
    data["last_log_messages"] = Log::last_logs();
    jsonResponse["data"] = data;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
