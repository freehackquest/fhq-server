#include "cmd_handlers.h"
#include <QtGlobal>
#include <fallen.h>
#include <employees.h>

// ---------------------------------------------------------------------

/*! 
 * WsjcppSocketClient - 
 * */

WsjcppSocketClient::WsjcppSocketClient(QWebSocket *pSocket) {
    TAG = "WsjcppSocketClient";
    m_pUserSession = nullptr;
    m_pSocket = pSocket;
    connect(m_pSocket, &QWebSocket::textMessageReceived, this, &WsjcppSocketClient::processTextMessage);
    connect(m_pSocket, &QWebSocket::binaryMessageReceived, this, &WsjcppSocketClient::processBinaryMessage);
    connect(m_pSocket, &QWebSocket::disconnected, this, &WsjcppSocketClient::socketDisconnected);
}

// ---------------------------------------------------------------------

WsjcppSocketClient::~WsjcppSocketClient() {
    m_pSocket->deleteLater();
}

// ---------------------------------------------------------------------

void WsjcppSocketClient::processTextMessage(const QString &message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    WsjcppLog::warn(TAG, "QWebSocket *pClient = " + WsjcppCore::getPointerAsHex(pClient));
    WsjcppLog::warn(TAG, "pClient->localPort() = " + std::to_string(pClient->localPort()));
    // processTextMessage
}

// ---------------------------------------------------------------------

void WsjcppSocketClient::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    WsjcppLog::warn(TAG, "QWebSocket *pClient = " + WsjcppCore::getPointerAsHex(pClient));
    WsjcppLog::warn(TAG, "pClient->localPort() = " + std::to_string(pClient->localPort()));
}

// ---------------------------------------------------------------------

void WsjcppSocketClient::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    WsjcppLog::warn(TAG, "QWebSocket *pClient = " + WsjcppCore::getPointerAsHex(pClient));
    WsjcppLog::warn(TAG, "pClient->localPort() = " + std::to_string(pClient->localPort()));

    // TODO hex print
    WsjcppLog::info(TAG, "socketDisconnected:" + WsjcppCore::getPointerAsHex(pClient));
    if (pClient) {
        // this->removeWsjcppJsonRpc20UserSession(pClient);
        // m_clients.removeAll(pClient);
        // pClient->deleteLater();
    }
}

// ---------------------------------------------------------------------

// ****************************
// **** WsjcppJsonRpc20Request
// ****************************


WsjcppJsonRpc20Request::WsjcppJsonRpc20Request(void *pClient, IWebSocketServer *pWebSocketServer, nlohmann::json &jsonRequest_) {
    TAG = "WsjcppJsonRpc20Request";
    m_pClient = pClient;
    m_pServer = pWebSocketServer;
    m_jsonRequest = jsonRequest_;

    if (m_jsonRequest["cmd"].is_string()) {
        m_sCommand = m_jsonRequest["cmd"];
    }

    if (m_jsonRequest["method"].is_string()) {
        m_sCommand = m_jsonRequest["method"];
    }

    if (m_jsonRequest["m"].is_string()) {
        m_sMessageId = m_jsonRequest["m"];
    }

    if (m_jsonRequest["id"].is_string()) {
        m_sMessageId = m_jsonRequest["id"];
    }

    m_pWsjcppJsonRpc20UserSession = m_pServer->findUserSession(m_pClient);
}

// ---------------------------------------------------------------------

QWebSocket *WsjcppJsonRpc20Request::client() {
    return (QWebSocket *)m_pClient;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Request::getIpAddress() {
    std::string sAddress = ((QWebSocket*)m_pClient)->peerAddress().toString().toStdString();
    #if QT_VERSION >= 0x050600
        QNetworkRequest r = ((QWebSocket*)m_pClient)->request();
        QByteArray qbaHeaderName = QString("x-forwarded-for").toLatin1();
        if (r.hasRawHeader(qbaHeaderName)) {
            sAddress = r.rawHeader(qbaHeaderName).toStdString();
        }
    #endif
    return sAddress;
}

// ---------------------------------------------------------------------

IWebSocketServer *WsjcppJsonRpc20Request::server() {
    return m_pServer;
}

// ---------------------------------------------------------------------

// TODO deprecated
const nlohmann::json& WsjcppJsonRpc20Request::jsonRequest() {
    return m_jsonRequest;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::hasInputParam(const std::string &sParamName) {
    if (m_jsonRequest.find(sParamName) != m_jsonRequest.end()) {
        return true;
    } 
    return false;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Request::getInputString(const std::string &sParamName, const std::string &sDefaultValue) {
    // TODO check by input defs
    std::string sRet = sDefaultValue;
    if (m_jsonRequest[sParamName].is_string()) {
        sRet = m_jsonRequest[sParamName];
    }
    return sRet;
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20Request::getInputInteger(const std::string &sParamName, int nDefaultValue) {
    // TODO check by input defs
    int nRet = nDefaultValue;
    if (m_jsonRequest[sParamName].is_number()) {
        nRet = m_jsonRequest[sParamName];
    }
    return nRet;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Request::m() {
    return m_sMessageId;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20UserSession *WsjcppJsonRpc20Request::getUserSession() {
    return m_pWsjcppJsonRpc20UserSession;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::isAdmin() {
    if (m_pWsjcppJsonRpc20UserSession != nullptr) {
        return m_pWsjcppJsonRpc20UserSession->isAdmin();
    }
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::isUser() {
    if (m_pWsjcppJsonRpc20UserSession != nullptr) {
        return m_pWsjcppJsonRpc20UserSession->isUser();
    }
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::isUnauthorized() {
    return m_pWsjcppJsonRpc20UserSession == nullptr;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20Request::sendMessageError(const std::string &cmd, WsjcppJsonRpc20Error error) {
    m_pServer->sendMessageError((QWebSocket*)m_pClient,cmd,m_sMessageId,error);
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20Request::sendMessageSuccess(const std::string &sMethod, nlohmann::json& jsonResponse) {
    jsonResponse["cmd"] = sMethod; // deprecated
    jsonResponse["jsonrpc"] = "2.0";
    jsonResponse["method"] = sMethod;
    jsonResponse["m"] = m_sMessageId;
    jsonResponse["result"] = "DONE";
    m_pServer->sendMessage((QWebSocket*)m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------
// TODO new for jsonrpc

void WsjcppJsonRpc20Request::sendResponse(nlohmann::json& jsonResult) {
    nlohmann::json jsonResponse;
    jsonResponse["jsonrpc"] = "2.0";
    jsonResponse["method"] = m_sCommand;
    jsonResponse["id"] = m_sMessageId;
    jsonResponse["result"] = jsonResult;
    m_pServer->sendMessage((QWebSocket*)m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::hasM() {
    return m_sMessageId != "";
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Request::command() {
    return m_sCommand;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::hasCommand() {
    return m_sCommand != "";
}

// ---------------------------------------------------------------------

// bool WsjcppJsonRpc20Request::validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler) {

// }

// ****************************
// **** CmdHandlerBase
// ****************************

// ---------------------------------------------------------------------

CmdHandlerBase::CmdHandlerBase(const std::string &sCmd, const std::string &sDescription) {
    m_sCmd = sCmd;
    m_sDescription = sDescription;
    TAG = "CmdHandlerBase(" + sCmd + ")";

    m_bAccessUnauthorized = false;
    m_bAccessUser = false;
    m_bAccessTester = false;
    m_bAccessAdmin = false;

    // can register in global variable
    CmdHandlers::addHandler(sCmd, this);
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::getActivatedFromVersion() const {
    return m_sActivatedFromVersion;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::getDeprecatedFromVersion() const {
    return m_sDeprecatedFromVersion;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::haveUnauthorizedAccess() const {
    return m_bAccessUnauthorized;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::haveUserAccess() const {
    return m_bAccessUser;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::haveTesterAccess() const {
    return m_bAccessTester;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::haveAdminAccess() const {
    return m_bAccessAdmin;
}

// ---------------------------------------------------------------------
// TODO write unit-test for this

bool CmdHandlerBase::checkAccess(WsjcppJsonRpc20Request *pRequest, WsjcppJsonRpc20Error& error) const {
    WsjcppJsonRpc20UserSession *pUserSession = pRequest->getUserSession();
    if (!haveUnauthorizedAccess()) {
        if (pUserSession == nullptr) {
            error = WsjcppJsonRpc20Error(401, "NOT_AUTHORIZED_REQUEST");
            return false;
        }

        // access user
        if (pUserSession->isUser() && !haveUserAccess()) {
            error = WsjcppJsonRpc20Error(403, "ACCESS_DENY_FOR_USER");
            return false;
        }

        // access tester
        if (pUserSession->isTester() && !haveTesterAccess()) {
            error = WsjcppJsonRpc20Error(403, "ACCESS_DENY_FOR_TESTER");
            return false;
        }

        // access admin
        if (pUserSession->isAdmin() && !haveAdminAccess()) {
            error = WsjcppJsonRpc20Error(403, "ACCESS_DENY_FOR_ADMIN");
            return false;
        }
    }

    return true;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::cmd() {
    return m_sCmd;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::getDescription() const {
    return m_sDescription;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessUnauthorized(bool bAccess) {
    m_bAccessUnauthorized = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessUser(bool bAccess) {
    m_bAccessUser = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessTester(bool bAccess) {
    m_bAccessTester = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessAdmin(bool bAccess) {
    m_bAccessAdmin = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setActivatedFromVersion(const std::string &sActivatedFromVersion) {
    m_sActivatedFromVersion = sActivatedFromVersion;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion) {
    m_sDeprecatedFromVersion = sDeprecatedFromVersion;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &CmdHandlerBase::requireStringParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    WsjcppJsonRpc20ParamDef pStringDef(sName, sDescription);
    pStringDef.string_().required();
    m_vInputs.push_back(pStringDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &CmdHandlerBase::optionalStringParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    WsjcppJsonRpc20ParamDef pStringDef(sName, sDescription);
    pStringDef.string_().optional();
    m_vInputs.push_back(pStringDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &CmdHandlerBase::requireIntegerParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    WsjcppJsonRpc20ParamDef pIntegerDef(sName, sDescription);
    pIntegerDef.integer_().required();
    m_vInputs.push_back(pIntegerDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &CmdHandlerBase::optionalIntegerParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    WsjcppJsonRpc20ParamDef pIntegerDef(sName, sDescription);
    pIntegerDef.integer_().optional();
    m_vInputs.push_back(pIntegerDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &CmdHandlerBase::requireBooleanParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    WsjcppJsonRpc20ParamDef pBooleanDef(sName, sDescription);
    pBooleanDef.bool_().required();
    m_vInputs.push_back(pBooleanDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &CmdHandlerBase::optionalBooleanParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    WsjcppJsonRpc20ParamDef pBooleanDef(sName, sDescription);
    pBooleanDef.bool_().optional();
    m_vInputs.push_back(pBooleanDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

const std::vector<WsjcppJsonRpc20ParamDef> &CmdHandlerBase::inputs() {
    return m_vInputs;
}

// ---------------------------------------------------------------------

/*
void CmdHandlerBase::success(nlohmann::json jsonResponse) {
    EmployWsServer *pEmployWsServer = findWsjcppEmploy<EmployWsServer>();
    // TODO sendMessageSuccess
    // and remove from WsjcppJsonRpc20Requests
}

// ---------------------------------------------------------------------

void CmdHandlerBase::error(int nCode, const std::string &sErrorMessage) {
    EmployWsServer *pEmployWsServer = findWsjcppEmploy<EmployWsServer>();
    // TODO sendMessageError
    // and remove from WsjcppJsonRpc20Requests

}
// ---------------------------------------------------------------------
*/

// ****************************
// **** CmdHandlers
// ****************************

std::map<std::string, CmdHandlerBase*> *g_pCmdHandlers = NULL;

void CmdHandlers::initGlobalVariables() {
    if (g_pCmdHandlers == NULL) {
        // WsjcppLog::info(std::string(), "Create handlers map");
        g_pCmdHandlers = new std::map<std::string, CmdHandlerBase*>();
    }
}

// ---------------------------------------------------------------------

void CmdHandlers::addHandler(const std::string &sCmd, CmdHandlerBase* pCmdHandler) {
    CmdHandlers::initGlobalVariables();
    if (g_pCmdHandlers->count(sCmd)) {
        WsjcppLog::err(sCmd, "Already registered");
    } else {
        g_pCmdHandlers->insert(std::pair<std::string, CmdHandlerBase*>(sCmd,pCmdHandler));
        // WsjcppLog::info(sCmd, "Registered");
    }
}

// ---------------------------------------------------------------------

CmdHandlerBase * CmdHandlers::findCmdHandler(const std::string &sCmd) {
    CmdHandlers::initGlobalVariables();
    CmdHandlerBase *pCmdHandler = NULL;

    if (g_pCmdHandlers->count(sCmd)) {
        pCmdHandler = g_pCmdHandlers->at(sCmd);
    }

    if (pCmdHandler == NULL) {
        WsjcppLog::err(sCmd, "Not found");
    }

    return pCmdHandler;
}

// ---------------------------------------------------------------------
// WJSCppCmdHandlerServerApi

REGISTRY_CMD(WJSCppCmdHandlerServerApi)

WJSCppCmdHandlerServerApi::WJSCppCmdHandlerServerApi()
    : CmdHandlerBase("server_api", "This method Will be return list of all handlers") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void WJSCppCmdHandlerServerApi::handle(WsjcppJsonRpc20Request *pRequest) {
    nlohmann::json jsonResponse;
    jsonResponse["version"] = WSJCPP_APP_VERSION; // TODO redesign, what?

    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    int nWsPort = pGlobalSettings->get("port").getNumberValue();
    bool bSslOn = pGlobalSettings->get("ssl_on").getBooleanValue();
    int nWssPort = pGlobalSettings->get("ssl_port").getNumberValue();

    jsonResponse["server_ws_port"] = pGlobalSettings->get("port").getNumberValue();
    if (bSslOn) {
        jsonResponse["server_wss_port"] = nWssPort;
    }

    nlohmann::json jsonHandlers = nlohmann::json::array();
    std::map<std::string, CmdHandlerBase *>::iterator it = g_pCmdHandlers->begin();
    while (it != g_pCmdHandlers->end()) {
        std::string sCmd = it->first;
        CmdHandlerBase *pHandler = g_pCmdHandlers->at(sCmd);

        nlohmann::json jsonHandler;

        jsonHandler["cmd"] = pHandler->cmd().c_str();
        jsonHandler["description"] = pHandler->getDescription();
        jsonHandler["access_unauthorized"] = pHandler->haveUnauthorizedAccess();
        jsonHandler["access_user"] = pHandler->haveUserAccess();
        jsonHandler["access_tester"] = pHandler->haveTesterAccess();
        jsonHandler["access_admin"] = pHandler->haveAdminAccess();

        nlohmann::json jsonInputs = nlohmann::json::array();
        std::vector<WsjcppJsonRpc20ParamDef> ins = pHandler->inputs();
        for (unsigned int i = 0; i < ins.size(); i++) {
            jsonInputs.push_back(ins[i].toJson());
        }
        jsonHandler["inputs"] = jsonInputs;
        jsonHandlers.push_back(jsonHandler);

        it++;
    }
    jsonResponse["data"] = jsonHandlers;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
