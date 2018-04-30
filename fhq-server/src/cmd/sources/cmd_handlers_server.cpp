#include <cmd_handlers_server.h>
#include <log.h>
#include <runtasks.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>

/*****************************************
 * This handler will be return list of handlers - publish api interfaces
 *****************************************/


CmdHandlerServerApi::CmdHandlerServerApi()
    : CmdHandlerBase("server_api", "This method Will be return list of all handlers"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerServerApi::handle(ModelRequest *pRequest){
    nlohmann::json jsonResponse;

    // EmploySettings *pSettings = findEmploy<EmploySettings>();

    // TODO
    /*result["port"] = m_pServerConfig->serverPort();
    result["ssl_port"] = m_pServerConfig->serverPort();*/

    auto jsonHandlers = nlohmann::json::array();
    std::map<std::string, CmdHandlerBase *>::iterator it = g_pCmdHandlers->begin();
    while(it != g_pCmdHandlers->end()){
        std::string sCmd = it->first;
        CmdHandlerBase *pHandler = g_pCmdHandlers->at(sCmd);

        nlohmann::json jsonHandler;

        jsonHandler["cmd"] = pHandler->cmd().c_str();
        jsonHandler["description"] = pHandler->description();
        jsonHandler["access_unauthorized"] = pHandler->access().accessUnauthorized();
        jsonHandler["access_user"] = pHandler->access().accessUser();
        jsonHandler["access_admin"] = pHandler->access().accessAdmin();

        auto jsonInputs = nlohmann::json::array();
        std::vector<CmdInputDef> ins = pHandler->inputs();
        for(unsigned int i = 0; i < ins.size(); i++){
            jsonInputs.push_back(ins[i].toJson());
        }
        jsonHandler["inputs"] = jsonInputs;
        jsonHandlers.push_back(jsonHandler);

        it++;
    }
    jsonResponse["data"] = jsonHandlers;
    jsonResponse["version"] = QCoreApplication::applicationVersion().toStdString();
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*****************************************
 * This handler will be return public info
 *****************************************/

CmdHandlerPublicInfo::CmdHandlerPublicInfo()
    : CmdHandlerBase("public_info", "Method return public information about server"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

}


// ---------------------------------------------------------------------

void CmdHandlerPublicInfo::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    nlohmann::json jsonQuests;
    auto jsonWinners = nlohmann::json::array();
    auto jsonCities = nlohmann::json::array();

    jsonQuests["count"] = pServerInfo->countQuests();
    jsonQuests["attempts"] = pServerInfo->countQuestsAttempt();
    jsonQuests["solved"] = pServerInfo->countQuestsCompleted();

    QSqlDatabase db = *(pDatabase->database());


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
            nlohmann::json city;
            city["cnt"] = record.value("cnt").toInt();
            city["city"] = record.value("city").toString().toHtmlEscaped().toStdString();
            jsonCities.push_back(city);
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
            nlohmann::json jsonUser;
            jsonUser["place"] = nPlace;
            jsonUser["nick"] = record.value("nick").toString().toHtmlEscaped().toStdString();
            jsonUser["university"] = record.value("university").toString().toHtmlEscaped().toStdString();
            jsonUser["rating"] = record.value("rating").toInt();
            jsonWinners.push_back(jsonUser);
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

CmdHandlerServerInfo::CmdHandlerServerInfo()
    : CmdHandlerBase("server_info", "Return server private information"){
    TAG = "CmdServerInfoHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerServerInfo::handle(ModelRequest *pRequest){
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();
    // QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;
    nlohmann::json data;

    nlohmann::json jsonRequestStatistics = pServerInfo->toJson(); // TODO how much db connections and time

    data["request_statistics"] = jsonRequestStatistics;
    data["server_started"] = pServerInfo->getServerStart().toString(Qt::ISODate).toStdString();
    qint64 updatime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    updatime = updatime - pServerInfo->getServerStart().toMSecsSinceEpoch();
    data["server_uptime_sec"] = updatime/1000;
    data["last_log_messages"] = Log::last_logs();
    jsonResponse["data"] = data;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*****************************************
 * This handler will be return private server settings
 *****************************************/

CmdHandlerServerSettings::CmdHandlerServerSettings()
    : CmdHandlerBase("server_settings", "Return server settings"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerServerSettings::handle(ModelRequest *pRequest){
    nlohmann::json jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    jsonResponse["data"] = pSettings->toJson(); // TODO how much db connections and time
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*****************************************
 * This handler for update private server settings
 *****************************************/

CmdHandlerServerSettingsUpdate::CmdHandlerServerSettingsUpdate()
    : CmdHandlerBase("server_settings_update", "Update server settings"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("name of setting"));
    m_vInputs.push_back(CmdInputDef("value").required().any_().description("value of setting"));
}

// ---------------------------------------------------------------------

void CmdHandlerServerSettingsUpdate::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    std::string sName = jsonRequest["name"].toString().toStdString();
    QString sNewValue = jsonRequest["value"].toString();

    if(!pSettings->hasSett(sName)){
        std::string sError = "Setting with name: " + sName + " did not found";
        pRequest->sendMessageError(cmd(), Errors::NotFound(QString(sError.c_str())));
        return;
    }

    std::string sType = pSettings->getSettType(sName);

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
