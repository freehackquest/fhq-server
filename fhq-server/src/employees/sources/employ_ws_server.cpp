#include <employ_ws_server.h>
#include <employ_server_config.h>
#include <employ_server_info.h>
#include <utils_logger.h>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QRegularExpression>

#include <regex>

REGISTRY_EMPLOY(EmployWsServer)

// ---------------------------------------------------------------------

EmployWsServer::EmployWsServer()
    : EmployBase(EmployWsServer::name(), {"start_ws_server", EmployServerConfig::name(), EmployServerInfo::name()}) {
    TAG = EmployWsServer::name();
    m_pWebSocketServer = NULL;
}

// ---------------------------------------------------------------------

bool EmployWsServer::init(){
    return true;
}

// ---------------------------------------------------------------------

void EmployWsServer::setServer(IWebSocketServer *pWebSocketServer){
    m_pWebSocketServer = pWebSocketServer;
}

// ---------------------------------------------------------------------

void EmployWsServer::sendToAll(const nlohmann::json& jsonMessage){
    m_pWebSocketServer->sendToAll(jsonMessage);
}

// ---------------------------------------------------------------------

void EmployWsServer::sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) {
    m_pWebSocketServer->sendToOne(pClient, jsonMessage);
}

// ---------------------------------------------------------------------

bool EmployWsServer::validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler, const nlohmann::json &jsonMessage){
    try {
        // TODO check extra params
        
        // for(auto &&inDef : pCmdHandler->inputs()){
        for (auto inDef : pCmdHandler->inputs()) { // TODO: when metheds 'is*' are marked as 'const', then add && to inDeff
            
            auto itJsonParamName = jsonMessage.find(inDef.getName());
            const auto endJson = jsonMessage.end();
            if (inDef.isRequired() && itJsonParamName == endJson) {
                error = Error(400, "Parameter '" + inDef.getName() + "' expected");
                return false;
            }

            if (itJsonParamName != endJson) {
                if (inDef.isInteger()) {
                    if (!itJsonParamName->is_number()) {
                        error = Error(400, "Parameter '" + inDef.getName() + "' must be integer");
                        return false;
                    }

                    int val = *itJsonParamName;
                    if (inDef.isMinVal() && val < inDef.getMinVal()) {
                        error = Error(400, "Parameter '" + inDef.getName() + "' must be more then " + std::to_string(inDef.getMinVal()));
                        return false;
                    }
                    if (inDef.isMaxVal() && val > inDef.getMaxVal()) {
                        error = Error(400, "Parameter '" + inDef.getName() + "' must be less then " + std::to_string(inDef.getMaxVal()));
                        return false;
                    }
                }

                if (inDef.isEnum()) {
                    QString val = QString::fromStdString(*itJsonParamName);
                    QStringList eList = inDef.getEnumList();
                    // TODO: redesign to validator
                    if (!eList.contains(val)) {
                        error = Error(400, "Parameter '" + inDef.getName() + "' expected values one from [" + eList.join(",").toStdString() + "]");
                        return false;
                    }
                }

                if (inDef.isUUID()) {
                    auto &&val = itJsonParamName->get_ref<std::string const&>();
                    // if expression is not valid then std::regex throws exception
                    std::regex rx("[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}");
                    if(!std::regex_match(val, rx)){
                        error = Error(400, "Wrong param '" + inDef.getName() + "': must be uuid");
                        return false;
                    }
                }

                if (inDef.isEmail()) {
                    auto &&val = itJsonParamName->get_ref<std::string const&>();
                    std::regex rx("^[0-9a-zA-Z]{1}[0-9a-zA-Z-._]*[0-9a-zA-Z]{1}@[0-9a-zA-Z]{1}[-.0-9a-zA-Z]*[0-9a-zA-Z]{1}\\.[a-zA-Z]{2,6}$");
                    if(!std::regex_match(val, rx)){
                        error = Error(400, "Parameter '" + inDef.getName() + "' must be email type");
                        return false;
                    }
                }
            }
        }
        return true;
    } catch(std::exception const &e) {
        error = Error(500, "InternalServerError");
        Log::err(TAG, std::string("validateInputParameters, ") + e.what());
        return false;
    }
}
