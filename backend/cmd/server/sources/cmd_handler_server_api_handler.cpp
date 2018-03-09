#include <cmd_handler_server_api.h>
#include <employees.h>
#include <employ_settings.h>
#include <cmd_handlers.h>
#include <QtCore>

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
