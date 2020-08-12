#include "cmd_handlers.h"
#include <QtGlobal>
#include <fallen.h>
#include <employees.h>

/*! 
 * WsjcppJsonRpc20Error - 
 * */

WsjcppJsonRpc20Error::WsjcppJsonRpc20Error(int nErrorCode, const std::string &sErrorMessage) {
    m_nErrorCode = nErrorCode;
    m_sErrorMessage = sErrorMessage;
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20Error::getErrorCode() {
    return m_nErrorCode;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Error::getErrorMessage() {
    return m_sErrorMessage;
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20Error::toJson() {
    nlohmann::json jsonRet;
    jsonRet["code"] = m_nErrorCode;
    jsonRet["message"] = m_sErrorMessage;
    return jsonRet;
}

// ---------------------------------------------------------------------

/*! 
 * WsjcppJsonRpc20UserSession - all data by current user session
 * */

WsjcppJsonRpc20UserSession::WsjcppJsonRpc20UserSession() {
    TAG = "WsjcppJsonRpc20UserSession";
    m_sSessionUuid = "";
    m_nUserID = -1;
    m_sUserRole = "";
    m_sUserEmail = "";
    m_sUserUuid = "";
    m_sUserName = "";
    m_sSessionUuid = "";
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20UserSession::WsjcppJsonRpc20UserSession(nlohmann::json const& obj) : WsjcppJsonRpc20UserSession() {
    this->fillFrom(obj);
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::fillFrom(const nlohmann::json &obj) {
    if (obj.find("user") != obj.end()) {
        nlohmann::json user = obj.at("user");

        // user.role
        try {
            m_sUserRole = user.at("role").get<std::string>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "JSON: " + obj.dump());
            WsjcppLog::err(TAG, "Something wrong param user.role in struct. " + std::string(e.what()));
            m_sUserRole = "";
        }

        // TODO check allow roles

        // user.id
        try {
            m_nUserID = user.at("id").get<int>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "JSON: " + obj.dump());
            WsjcppLog::err(TAG, "Something wrong param user.id in struct. " + std::string(e.what()));
            m_nUserID = -1;
        }
        
        // user.email
        try {
            m_sUserEmail = user.at("email").get<std::string>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "JSON: " + obj.dump());
            WsjcppLog::err(TAG, "Something wrong param user.email in struct. " + std::string(e.what()));
            m_sUserEmail = "";
        }

        // user.nick
        try {
            m_sUserName = user.at("nick").get<std::string>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "JSON: " + obj.dump());
            WsjcppLog::err(TAG, "Something wrong param user.nick in struct. " + std::string(e.what()));
            m_sUserName = "";
        }

        // user.uuid
        try {
            m_sUserUuid = user.at("uuid").get<std::string>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "JSON: " + obj.dump());
            WsjcppLog::err(TAG, "Something wrong param user.uuid in struct. " + std::string(e.what()));
            m_sUserUuid = "";
        }
        
    } else {
        WsjcppLog::warn(TAG, "Not found param 'user' in struct");
    }
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getSessionUuid() {
    return m_sSessionUuid;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionUuid(const std::string& sSessionUuid) {
    m_sSessionUuid = sSessionUuid;
}

// ---------------------------------------------------------------------

long WsjcppJsonRpc20UserSession::getSessionCreated() {
    return m_nSessionCreated;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionCreated(long nSessionCreated) {
    m_nSessionCreated = nSessionCreated;
}

// ---------------------------------------------------------------------

long WsjcppJsonRpc20UserSession::getSessionUpdated() {
    return m_nSessionUpdated;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionUpdated(long nSessionUpdated) {
    m_nSessionUpdated = nSessionUpdated;
}

// ---------------------------------------------------------------------

long WsjcppJsonRpc20UserSession::getSessionExpire() {
    return m_nSessionExpire;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionExpire(long nSessionExpire) {
    m_nSessionExpire = nSessionExpire;
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20UserSession::getUserId() {
    return m_nUserID;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserId(int nUserId) {
    m_nUserID = nUserId;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getUserUuid() {
    return m_sUserUuid;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserUuid(const std::string& sUserUuid) {
    m_sUserUuid = sUserUuid;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getUserName() {
    return m_sUserName;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserName(const std::string &sUserName) {
    m_sUserName = sUserName;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getUserEmail() {
    return m_sUserEmail;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserEmail(const std::string &sUserEmail) {
    m_sUserEmail = sUserEmail;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getUserRole() {
    return m_sUserRole;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserRole(const std::string &sUserRole) {
    m_sUserRole = sUserRole;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20UserSession::isAdmin() {
    return m_sUserRole == "admin";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20UserSession::isUser() {
    return m_sUserRole == "user";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20UserSession::isTester() {
    return m_sUserRole == "tester";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20UserSession::hasRole() {
    return m_sUserRole != "";
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20UserSession::getUserCustom() {
    return m_jsonUserCustom;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserCustom(const nlohmann::json &jsonCustom) {
    m_jsonUserCustom = jsonCustom;
}

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

// ****************************
// **** WsjcppJsonRpc20ParamDef
// ****************************

WsjcppJsonRpc20ParamDef::WsjcppJsonRpc20ParamDef(const std::string &sName, const std::string &sDescription) {
    m_sName = sName;
    m_sDescription = sDescription;
    m_bSettedMinVal = false;
    m_bSettedMaxVal = false;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef::WsjcppJsonRpc20ParamDef() {
    m_bSettedMinVal = false;
    m_bSettedMaxVal = false;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::optional() {
    m_sRestrict = "optional";
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::required() {
    m_sRestrict = "required";
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::string_() {
    m_sType = std::string(CMD_INPUT_DEF_TYPE_STRING);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::integer_() {
    m_sType = CMD_INPUT_DEF_TYPE_INTEGER;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::bool_() {
    m_sType = std::string(CMD_INPUT_DEF_TYPE_BOOL);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::any_() {
    m_sType = std::string(CMD_INPUT_DEF_TYPE_ANY);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::description(const std::string& s) {
    m_sDescription = s;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::minval(int minval) {
    m_bSettedMinVal = true;
    m_nMinVal = minval;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::maxval(int maxval) {
    m_bSettedMaxVal = true;
    m_nMaxVal = maxval;
    return *this;
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20ParamDef::toJson() {
    nlohmann::json obj;
    obj["name"] = m_sName;
    obj["type"] = m_sType;
    obj["restrict"] = m_sRestrict;
    obj["description"] = m_sDescription;
    // TODO validator description
    return obj;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getType() {
    return m_sType;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getType() const {
    return m_sType;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getName() const {
    return m_sName;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getRestrict() {
    return m_sRestrict;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getRestrict() const {
    return m_sRestrict;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getDescription() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getDescription() const {
    return m_sDescription;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isRequired() {
    return m_sRestrict == "required";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isInteger() {
    return m_sType == CMD_INPUT_DEF_TYPE_INTEGER;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isString() {
    return m_sType == CMD_INPUT_DEF_TYPE_STRING;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isBool() {
    return m_sType == CMD_INPUT_DEF_TYPE_BOOL;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isAny() {
    return m_sType == CMD_INPUT_DEF_TYPE_ANY;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isMinVal() {
    return m_bSettedMaxVal;
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20ParamDef::getMinVal() {
    return m_nMinVal;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isMaxVal() {
    return m_bSettedMaxVal;
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20ParamDef::getMaxVal() {
    return m_nMaxVal;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppValidatorStringBase *> &WsjcppJsonRpc20ParamDef::listOfValidators() {
    return m_vValidatorsString;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20ParamDef::addValidator(WsjcppValidatorStringBase *pValidatorStringBase) {
    // TODO check type
    m_vValidatorsString.push_back(pValidatorStringBase);
    return *this;
}

// ---------------------------------------------------------------------

// ****************************
// **** WsjcppJsonRpc20Request
// ****************************


WsjcppJsonRpc20Request::WsjcppJsonRpc20Request(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, nlohmann::json &jsonRequest_) {
    TAG = "WsjcppJsonRpc20Request";
    m_pClient = pClient;
    m_pServer = pWebSocketServer;
    m_jsonRequest = jsonRequest_;

    if (m_jsonRequest["cmd"].is_string()) {
        m_sCommand = m_jsonRequest["cmd"];
    }

    if (m_jsonRequest["m"].is_string()) {
        m_sMessageId = m_jsonRequest["m"];
    }

    m_pWsjcppJsonRpc20UserSession = m_pServer->getWsjcppJsonRpc20UserSession(m_pClient);
}

// ---------------------------------------------------------------------

QWebSocket *WsjcppJsonRpc20Request::client() {
    return m_pClient;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Request::getIpAddress() {
    std::string sAddress = m_pClient->peerAddress().toString().toStdString();
    #if QT_VERSION >= 0x050600
        QNetworkRequest r = m_pClient->request();
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
    m_pServer->sendMessageError(m_pClient,cmd,m_sMessageId,error);
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20Request::sendMessageSuccess(const std::string &sMethod, nlohmann::json& jsonResponse) {
    jsonResponse["cmd"] = sMethod; // deprecated
    jsonResponse["jsonrpc"] = "2.0";
    jsonResponse["method"] = sMethod;
    jsonResponse["m"] = m_sMessageId;
    jsonResponse["result"] = "DONE";
    m_pServer->sendMessage(m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------
// TODO new for jsonrpc

void WsjcppJsonRpc20Request::sendResponse(nlohmann::json& jsonResult) {
    nlohmann::json jsonResponse;
    jsonResponse["jsonrpc"] = "2.0";
    jsonResponse["method"] = m_sCommand;
    jsonResponse["id"] = m_sMessageId;
    jsonResponse["result"] = jsonResult;
    m_pServer->sendMessage(m_pClient, jsonResponse);
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
    m_bAccessAdmin = false;

    // can register in global variable
    CmdHandlers::addHandler(sCmd, this);
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::activatedFromVersion() {
    return m_sActivatedFromVersion;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::deprecatedFromVersion() {
    return m_sDeprecatedFromVersion;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::accessUnauthorized() {
    return m_bAccessUnauthorized;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::accessUser() {
    return m_bAccessUser;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::accessAdmin() {
    return m_bAccessAdmin;
}

// ---------------------------------------------------------------------
// TODO write unit-test for this

bool CmdHandlerBase::checkAccess(WsjcppJsonRpc20Request *pRequest) {
    WsjcppJsonRpc20UserSession *pUserSession = pRequest->getUserSession();
    if (!accessUnauthorized()) {
        if (pUserSession == nullptr) {
            pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(401, "Not Authorized Request"));
            return false;
        }

        // access user
        if (pUserSession->isUser() && !accessUser()) {
            pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(403, "Access deny for user"));
            return false;
        }

        // access admin
        if (pUserSession->isAdmin() && !accessAdmin()) {
            pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(403, "Access deny for admin"));
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

std::string CmdHandlerBase::description() {
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
        jsonHandler["description"] = pHandler->description();
        jsonHandler["access_unauthorized"] = pHandler->accessUnauthorized();
        jsonHandler["access_user"] = pHandler->accessUser();
        jsonHandler["access_admin"] = pHandler->accessAdmin();

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
