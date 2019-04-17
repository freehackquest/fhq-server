#include <employ_ws_server.h>
#include <employ_server_config.h>
#include <employ_server_info.h>
#include <utils_logger.h>

REGISTRY_EMPLOY(EmployWsServer)

// ---------------------------------------------------------------------

EmployWsServer::EmployWsServer()
    : EmployBase(EmployWsServer::name(), {"start_ws_server", EmployServerConfig::name(), EmployServerInfo::name()}) {
    TAG = EmployWsServer::name();
    m_pWebSocketServer = NULL;
}

// ---------------------------------------------------------------------

bool EmployWsServer::init() {
    return true;
}

// ---------------------------------------------------------------------

void EmployWsServer::setServer(IWebSocketServer *pWebSocketServer) {
    m_pWebSocketServer = pWebSocketServer;
}

// ---------------------------------------------------------------------

void EmployWsServer::sendToAll(const nlohmann::json& jsonMessage) {
    m_pWebSocketServer->sendToAll(jsonMessage);
}

// ---------------------------------------------------------------------

void EmployWsServer::sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) {
    m_pWebSocketServer->sendToOne(pClient, jsonMessage);
}

// ---------------------------------------------------------------------

bool EmployWsServer::validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler, const nlohmann::json &jsonMessage) {
    try {
        // TODO check extra params

        for (CmdInputDef inDef : pCmdHandler->inputs()) {
            
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

                if (inDef.isString()) {
                    std::string sVal = itJsonParamName->get_ref<std::string const&>();
                    std::string sError;
                    const std::vector<ValidatorStringBase *> vValidators = inDef.listOfValidators();
                    for (int i = 0; i < vValidators.size(); i++) {
                        if (!vValidators[i]->isValid(sVal, sError)) {
                            error = Error(400, "Wrong param '" + inDef.getName() + "': " + sError);
                            return false;
                        }
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
