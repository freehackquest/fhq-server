#include <cmd_handlers_server.h>
#include <log.h>
#include <runtasks.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <memory_cache_serverinfo.h>
#include <QtCore>

/*****************************************
 * This handler will be return list of handlers - publish api interfaces
 *****************************************/


CmdHandlerServerApi::CmdHandlerServerApi(){
    TAG = "CmdHandlerServerApi";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

std::string CmdHandlerServerApi::cmd(){
    return "server_api";
}

// ---------------------------------------------------------------------

std::string CmdHandlerServerApi::description(){
    return "This method Will be return list of all handlers";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerServerApi::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerServerApi::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

void CmdHandlerServerApi::handle(ModelRequest *pRequest){
    QJsonObject jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();
    QJsonObject data;
    // pRequest->server()->exportApi(data);

    // TODO
    /*result["port"] = m_pServerConfig->serverPort();
    result["ssl_port"] = m_pServerConfig->serverPort();*/


    QJsonArray handlers;
    std::map<std::string, ICmdHandler *>::iterator it = g_pCmdHandlers->begin();
    while(it != g_pCmdHandlers->end()){
        std::string sCmd = it->first;
        ICmdHandler *pHandler = g_pCmdHandlers->at(sCmd);

        QJsonObject handler;

        handler["cmd"] = QString(pHandler->cmd().c_str());
        handler["description"] = QString(pHandler->description().c_str());
        handler["access_unauthorized"] = pHandler->access().accessUnauthorized();
        handler["access_user"] = pHandler->access().accessUser();
        handler["access_admin"] = pHandler->access().accessAdmin();

        QJsonArray inputs;
        std::vector<CmdInputDef> ins = pHandler->inputs();
        for(unsigned int i = 0; i < ins.size(); i++){
            inputs.append(ins[i].toJson());
        }
        handler["inputs"] = inputs;
        handlers.append(handler);

        it++;
    }
    jsonResponse["data"] = handlers;
    jsonResponse["version"] = QCoreApplication::applicationVersion();
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*****************************************
 * This handler will be return public info
 *****************************************/

CmdPublicInfoHandler::CmdPublicInfoHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

}

// ---------------------------------------------------------------------

std::string CmdPublicInfoHandler::cmd(){
    return "public_info";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdPublicInfoHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdPublicInfoHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdPublicInfoHandler::description(){
    return "Method return public information about server";
}

// ---------------------------------------------------------------------

void CmdPublicInfoHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QJsonObject jsonQuests;
    QJsonArray jsonWinners;
    QJsonArray jsonCities;

    // fill quests info
    IMemoryCache *pMemoryCache = pRequest->server()->findMemoryCache("serverinfo");
    if(pMemoryCache == NULL){
        pRequest->sendMessageError(cmd(), Errors::InternalServerError());
        return;
    }
    MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);
    jsonQuests["count"] = pMemoryCacheServerInfo->countQuests();
    jsonQuests["attempts"] = pMemoryCacheServerInfo->countQuestsAttempt();
    jsonQuests["solved"] = pMemoryCacheServerInfo->countQuestsCompleted();

    QSqlDatabase db = *(pRequest->server()->database());


    int nMoreThen = 2;
    int nCitiesLimit = 20;

    // TODO load from settings
    // nMoreThen
    // nCitiesLimit

    // cities
    // TODO get from cache
    {
        QSqlQuery query(db);
        query.prepare(""
                      "SELECT * FROM ("
                      "     SELECT city, COUNT(*) cnt FROM users WHERE city IS NOT NULL AND city <> '' GROUP BY city ORDER BY cnt DESC "
                      ") AS cities "
                      "WHERE cities.cnt > :morethan "
                      "LIMIT 0,:citieslimit");
        query.bindValue(":morethan", nMoreThen);
        query.bindValue(":citieslimit", nCitiesLimit);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        while(query.next()) {
            QSqlRecord record = query.record();
            QJsonObject city;
            city["cnt"] = record.value("cnt").toInt();
            city["city"] = record.value("city").toString().toHtmlEscaped();
            jsonCities.append(city);
        }
    }



    // TODO get from cache
    {
        QSqlQuery query(db);
        query.prepare("SELECT u.nick, u.university, u.rating FROM users u WHERE u.role = :role ORDER BY u.rating DESC LIMIT 0,10");
        query.bindValue(":role", "user");
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        int nPlace = 1;
        while(query.next()) {
            QSqlRecord record = query.record();
            QJsonObject user;
            user["place"] = nPlace;
            user["nick"] = record.value("nick").toString().toHtmlEscaped();
            user["university"] = record.value("university").toString().toHtmlEscaped();
            user["rating"] = record.value("rating").toInt();
            jsonWinners.append(user);
            nPlace++;
        }
    }

    jsonResponse["quests"] = jsonQuests;
    jsonResponse["winners"] = jsonWinners;
    jsonResponse["cities"] = jsonCities;
    jsonResponse["connectedusers"] = pRequest->server()->getConnectedUsers();
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*****************************************
 * This handler will be return private server info
 *****************************************/

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

/*****************************************
 * This handler will be return private server settings
 *****************************************/

CmdServerSettingsHandler::CmdServerSettingsHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

std::string CmdServerSettingsHandler::cmd(){
        return "server_settings";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdServerSettingsHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdServerSettingsHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdServerSettingsHandler::description(){
        return "Return server settings";
}

// ---------------------------------------------------------------------

void CmdServerSettingsHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    jsonResponse["data"] = pSettings->toJsonArray(); // TODO how much db connections and time
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*****************************************
 * This handler for update private server settings
 *****************************************/

CmdServerSettingsUpdateHandler::CmdServerSettingsUpdateHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("name of setting"));
    m_vInputs.push_back(CmdInputDef("value").required().any_().description("value of setting"));
}

// ---------------------------------------------------------------------

std::string CmdServerSettingsUpdateHandler::cmd(){
        return "server_settings_update";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdServerSettingsUpdateHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdServerSettingsUpdateHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdServerSettingsUpdateHandler::description(){
        return "Update server settings";
}

// ---------------------------------------------------------------------

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
