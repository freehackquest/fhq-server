#include "cmd_handlers.h"
#include <QtGlobal>
#include <fallen.h>
#include <wsjcpp_employees.h>

/*! 
 * WSJCppError - 
 * */

WSJCppError::WSJCppError(int nCodeError, const std::string &sMessage) {
    m_nCodeError = nCodeError;
    m_sMessage = sMessage;
}

// ---------------------------------------------------------------------

int WSJCppError::codeError() {
    return m_nCodeError;
}

// ---------------------------------------------------------------------

std::string WSJCppError::message() {
    return m_sMessage;
}

// ---------------------------------------------------------------------

/*! 
 * WSJCppUserSession - all data by current user session
 * */

WSJCppUserSession::WSJCppUserSession() {
    TAG = "WSJCppUserSession";
    m_nUserID = -1;
    m_sRole = "";
    m_sEmail = "";
    m_sNick = "";
    m_sUserUuid = "";
}

// ---------------------------------------------------------------------

WSJCppUserSession::WSJCppUserSession(nlohmann::json const& obj) : WSJCppUserSession() {
    this->fillFrom(obj);
}

// ---------------------------------------------------------------------

void WSJCppUserSession::fillFrom(const nlohmann::json &obj) {
    if (obj.find("user") != obj.end()) {
        nlohmann::json user = obj.at("user");

        // user.role
        try {
            m_sRole = user.at("role").get<std::string>();
        } catch (const std::exception &e) {
            WSJCppLog::err(TAG, "JSON: " + obj.dump());
            WSJCppLog::err(TAG, "Something wrong param user.role in struct. " + std::string(e.what()));
            m_sRole = "";
        }

        // TODO check allow roles

        // user.id
        try {
            m_nUserID = user.at("id").get<int>();
        } catch (const std::exception &e) {
            WSJCppLog::err(TAG, "JSON: " + obj.dump());
            WSJCppLog::err(TAG, "Something wrong param user.id in struct. " + std::string(e.what()));
            m_nUserID = -1;
        }
        
        // user.email
        try {
            m_sEmail = user.at("email").get<std::string>();
        } catch (const std::exception &e) {
            WSJCppLog::err(TAG, "JSON: " + obj.dump());
            WSJCppLog::err(TAG, "Something wrong param user.email in struct. " + std::string(e.what()));
            m_sEmail = "";
        }

        // user.nick
        try {
            m_sNick = user.at("nick").get<std::string>();
        } catch (const std::exception &e) {
            WSJCppLog::err(TAG, "JSON: " + obj.dump());
            WSJCppLog::err(TAG, "Something wrong param user.nick in struct. " + std::string(e.what()));
            m_sNick = "";
        }

        // user.uuid
        try {
            m_sUserUuid = user.at("uuid").get<std::string>();
        } catch (const std::exception &e) {
            WSJCppLog::err(TAG, "JSON: " + obj.dump());
            WSJCppLog::err(TAG, "Something wrong param user.uuid in struct. " + std::string(e.what()));
            m_sUserUuid = "";
        }
        
    } else {
        WSJCppLog::warn(TAG, "Not found param 'user' in struct");
    }
}

// ---------------------------------------------------------------------

bool WSJCppUserSession::isAdmin() {
    return m_sRole == "admin";
}

// ---------------------------------------------------------------------

bool WSJCppUserSession::isUser() {
    return m_sRole == "user";
}

// ---------------------------------------------------------------------

bool WSJCppUserSession::isTester() {
    return m_sRole == "tester";
}

// ---------------------------------------------------------------------

bool WSJCppUserSession::hasRole() {
    return m_sRole != "";
}

// ---------------------------------------------------------------------

QString WSJCppUserSession::nick() {
    return QString::fromStdString(m_sNick);
}

// ---------------------------------------------------------------------

void WSJCppUserSession::setNick(QString sNick) {
    m_sNick = sNick.toStdString();
}

// ---------------------------------------------------------------------

int WSJCppUserSession::userid() {
    return m_nUserID;
}

std::string WSJCppUserSession::userUuid() {
    return m_sUserUuid;
}

// ---------------------------------------------------------------------

QString WSJCppUserSession::email() {
    return QString::fromStdString(m_sEmail);
}


/*! 
 * WSJCppSocketClient - 
 * */

WSJCppSocketClient::WSJCppSocketClient(QWebSocket *pSocket) {
    TAG = "WSJCppSocketClient";
    m_pUserSession = nullptr;
    m_pSocket = pSocket;
    connect(m_pSocket, &QWebSocket::textMessageReceived, this, &WSJCppSocketClient::processTextMessage);
    connect(m_pSocket, &QWebSocket::binaryMessageReceived, this, &WSJCppSocketClient::processBinaryMessage);
    connect(m_pSocket, &QWebSocket::disconnected, this, &WSJCppSocketClient::socketDisconnected);
}

// ---------------------------------------------------------------------

WSJCppSocketClient::~WSJCppSocketClient() {
    m_pSocket->deleteLater();
}

// ---------------------------------------------------------------------

void WSJCppSocketClient::processTextMessage(const QString &message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    WSJCppLog::warn(TAG, "QWebSocket *pClient = " + WSJCppCore::getPointerAsHex(pClient));
    WSJCppLog::warn(TAG, "pClient->localPort() = " + std::to_string(pClient->localPort()));
    // processTextMessage
}

// ---------------------------------------------------------------------

void WSJCppSocketClient::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    WSJCppLog::warn(TAG, "QWebSocket *pClient = " + WSJCppCore::getPointerAsHex(pClient));
    WSJCppLog::warn(TAG, "pClient->localPort() = " + std::to_string(pClient->localPort()));
}

// ---------------------------------------------------------------------

void WSJCppSocketClient::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    WSJCppLog::warn(TAG, "QWebSocket *pClient = " + WSJCppCore::getPointerAsHex(pClient));
    WSJCppLog::warn(TAG, "pClient->localPort() = " + std::to_string(pClient->localPort()));

    // TODO hex print
    WSJCppLog::info(TAG, "socketDisconnected:" + WSJCppCore::getPointerAsHex(pClient));
    if (pClient) {
        // this->removeWSJCppUserSession(pClient);
        // m_clients.removeAll(pClient);
        // pClient->deleteLater();
    }
}

// ****************************
// **** CmdInputDef
// ****************************

CmdInputDef::CmdInputDef(const std::string &sName, const std::string &sDescription) {
    m_sName = sName;
    m_sDescription = sDescription;
    m_bSettedMinVal = false;
    m_bSettedMaxVal = false;
}

// ---------------------------------------------------------------------

CmdInputDef::CmdInputDef() {
    m_bSettedMinVal = false;
    m_bSettedMaxVal = false;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::optional() {
    m_sRestrict = "optional";
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::required() {
    m_sRestrict = "required";
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::string_() {
    m_sType = std::string(CMD_INPUT_DEF_TYPE_STRING);
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::integer_() {
    m_sType = CMD_INPUT_DEF_TYPE_INTEGER;
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::bool_() {
    m_sType = std::string(CMD_INPUT_DEF_TYPE_BOOL);
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::any_() {
    m_sType = std::string(CMD_INPUT_DEF_TYPE_ANY);
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::description(const std::string& s) {
    m_sDescription = s;
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::minval(int minval) {
    m_bSettedMinVal = true;
    m_nMinVal = minval;
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::maxval(int maxval) {
    m_bSettedMaxVal = true;
    m_nMaxVal = maxval;
    return *this;
}

// ---------------------------------------------------------------------

nlohmann::json CmdInputDef::toJson() {
    nlohmann::json obj;
    obj["name"] = m_sName;
    obj["type"] = m_sType;
    obj["restrict"] = m_sRestrict;
    obj["description"] = m_sDescription;
    // TODO validator description
    return obj;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getType() {
    return m_sType;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getType() const {
    return m_sType;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getName() const {
    return m_sName;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getRestrict() {
    return m_sRestrict;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getRestrict() const {
    return m_sRestrict;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getDescription() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getDescription() const {
    return m_sDescription;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isRequired() {
    return m_sRestrict == "required";
}

// ---------------------------------------------------------------------

bool CmdInputDef::isInteger() {
    return m_sType == CMD_INPUT_DEF_TYPE_INTEGER;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isString() {
    return m_sType == CMD_INPUT_DEF_TYPE_STRING;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isBool() {
    return m_sType == CMD_INPUT_DEF_TYPE_BOOL;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isAny() {
    return m_sType == CMD_INPUT_DEF_TYPE_ANY;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isMinVal() {
    return m_bSettedMaxVal;
}

// ---------------------------------------------------------------------

int CmdInputDef::getMinVal() {
    return m_nMinVal;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isMaxVal() {
    return m_bSettedMaxVal;
}

// ---------------------------------------------------------------------

int CmdInputDef::getMaxVal() {
    return m_nMaxVal;
}

// ---------------------------------------------------------------------

const std::vector<WSJCppValidatorStringBase *> &CmdInputDef::listOfValidators() {
    return m_vValidatorsString;
}

// ---------------------------------------------------------------------

CmdInputDef &CmdInputDef::addValidator(WSJCppValidatorStringBase *pValidatorStringBase) {
    // TODO check type
    m_vValidatorsString.push_back(pValidatorStringBase);
    return *this;
}

// ---------------------------------------------------------------------

// ****************************
// **** ModelRequest
// ****************************


ModelRequest::ModelRequest(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, nlohmann::json &jsonRequest_) {
    TAG = "ModelRequest";
    m_pClient = pClient;
    m_pServer = pWebSocketServer;
    m_jsonRequest = jsonRequest_;

    if (m_jsonRequest["cmd"].is_string()) {
        m_sCommand = m_jsonRequest["cmd"];
    }

    if (m_jsonRequest["m"].is_string()) {
        m_sMessageId = m_jsonRequest["m"];
    }

    m_pWSJCppUserSession = m_pServer->getWSJCppUserSession(m_pClient);
}

// ---------------------------------------------------------------------

QWebSocket *ModelRequest::client() {
    return m_pClient;
}

// ---------------------------------------------------------------------

std::string ModelRequest::getIpAddress() {
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

IWebSocketServer *ModelRequest::server() {
    return m_pServer;
}

// ---------------------------------------------------------------------

// TODO deprecated
const nlohmann::json& ModelRequest::jsonRequest() {
    return m_jsonRequest;
}

// ---------------------------------------------------------------------

bool ModelRequest::hasInputParam(const std::string &sParamName) {
    if (m_jsonRequest.find(sParamName) != m_jsonRequest.end()) {
        return true;
    } 
    return false;
}

// ---------------------------------------------------------------------

std::string ModelRequest::getInputString(const std::string &sParamName, const std::string &sDefaultValue) {
    // TODO check by input defs
    std::string sRet = sDefaultValue;
    if (m_jsonRequest[sParamName].is_string()) {
        sRet = m_jsonRequest[sParamName];
    }
    return sRet;
}

// ---------------------------------------------------------------------

int ModelRequest::getInputInteger(const std::string &sParamName, int nDefaultValue) {
    // TODO check by input defs
    int nRet = nDefaultValue;
    if (m_jsonRequest[sParamName].is_number()) {
        nRet = m_jsonRequest[sParamName];
    }
    return nRet;
}

// ---------------------------------------------------------------------

std::string ModelRequest::m() {
    return m_sMessageId;
}

// ---------------------------------------------------------------------

WSJCppUserSession *ModelRequest::getUserSession() {
    return m_pWSJCppUserSession;
}

// ---------------------------------------------------------------------

bool ModelRequest::isAdmin() {
    if (m_pWSJCppUserSession != nullptr) {
        return m_pWSJCppUserSession->isAdmin();
    }
    return false;
}

// ---------------------------------------------------------------------

bool ModelRequest::isUser() {
    if (m_pWSJCppUserSession != nullptr) {
        return m_pWSJCppUserSession->isUser();
    }
    return false;
}

// ---------------------------------------------------------------------

bool ModelRequest::isUnauthorized() {
    return m_pWSJCppUserSession == nullptr;
}

// ---------------------------------------------------------------------

void ModelRequest::sendMessageError(const std::string &cmd, WSJCppError error) {
    m_pServer->sendMessageError(m_pClient,cmd,m_sMessageId,error);
}

// ---------------------------------------------------------------------

void ModelRequest::sendMessageSuccess(const std::string &sMethod, nlohmann::json& jsonResponse) {
    jsonResponse["cmd"] = sMethod; // deprecated
    jsonResponse["jsonrpc"] = "2.0";
    jsonResponse["method"] = sMethod;
    jsonResponse["m"] = m_sMessageId;
    jsonResponse["result"] = "DONE";
    m_pServer->sendMessage(m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------
// TODO new for jsonrpc

void ModelRequest::sendResponse(nlohmann::json& jsonResult) {
    nlohmann::json jsonResponse;
    jsonResponse["jsonrpc"] = "2.0";
    jsonResponse["method"] = m_sCommand;
    jsonResponse["id"] = m_sMessageId;
    jsonResponse["result"] = jsonResult;
    m_pServer->sendMessage(m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------

bool ModelRequest::hasM() {
    return m_sMessageId != "";
}

// ---------------------------------------------------------------------

std::string ModelRequest::command() {
    return m_sCommand;
}

// ---------------------------------------------------------------------

bool ModelRequest::hasCommand() {
    return m_sCommand != "";
}

// ---------------------------------------------------------------------

// bool ModelRequest::validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler) {

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

bool CmdHandlerBase::checkAccess(ModelRequest *pRequest) {
    WSJCppUserSession *pUserSession = pRequest->getUserSession();
    if (!accessUnauthorized()) {
        if (pUserSession == nullptr) {
            pRequest->sendMessageError(cmd(), WSJCppError(401, "Not Authorized Request"));
            return false;
        }

        // access user
        if (pUserSession->isUser() && !accessUser()) {
            pRequest->sendMessageError(cmd(), WSJCppError(403, "Access deny for user"));
            return false;
        }

        // access admin
        if (pUserSession->isAdmin() && !accessAdmin()) {
            pRequest->sendMessageError(cmd(), WSJCppError(403, "Access deny for admin"));
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

CmdInputDef &CmdHandlerBase::requireStringParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pStringDef(sName, sDescription);
    pStringDef.string_().required();
    m_vInputs.push_back(pStringDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::optionalStringParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pStringDef(sName, sDescription);
    pStringDef.string_().optional();
    m_vInputs.push_back(pStringDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::requireIntegerParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pIntegerDef(sName, sDescription);
    pIntegerDef.integer_().required();
    m_vInputs.push_back(pIntegerDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::optionalIntegerParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pIntegerDef(sName, sDescription);
    pIntegerDef.integer_().optional();
    m_vInputs.push_back(pIntegerDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::requireBooleanParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pBooleanDef(sName, sDescription);
    pBooleanDef.bool_().required();
    m_vInputs.push_back(pBooleanDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::optionalBooleanParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pBooleanDef(sName, sDescription);
    pBooleanDef.bool_().optional();
    m_vInputs.push_back(pBooleanDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerBase::inputs() {
    return m_vInputs;
}

// ---------------------------------------------------------------------

/*
void CmdHandlerBase::success(nlohmann::json jsonResponse) {
    EmployWsServer *pEmployWsServer = findEmploy<EmployWsServer>();
    // TODO sendMessageSuccess
    // and remove from ModelRequests
}

// ---------------------------------------------------------------------

void CmdHandlerBase::error(int nCode, const std::string &sErrorMessage) {
    EmployWsServer *pEmployWsServer = findEmploy<EmployWsServer>();
    // TODO sendMessageError
    // and remove from ModelRequests

}
// ---------------------------------------------------------------------
*/

// ****************************
// **** CmdHandlers
// ****************************

std::map<std::string, CmdHandlerBase*> *g_pCmdHandlers = NULL;

void CmdHandlers::initGlobalVariables() {
    if (g_pCmdHandlers == NULL) {
        // WSJCppLog::info(std::string(), "Create handlers map");
        g_pCmdHandlers = new std::map<std::string, CmdHandlerBase*>();
    }
}

// ---------------------------------------------------------------------

void CmdHandlers::addHandler(const std::string &sCmd, CmdHandlerBase* pCmdHandler) {
    CmdHandlers::initGlobalVariables();
    if (g_pCmdHandlers->count(sCmd)) {
        WSJCppLog::err(sCmd, "Already registered");
    } else {
        g_pCmdHandlers->insert(std::pair<std::string, CmdHandlerBase*>(sCmd,pCmdHandler));
        // WSJCppLog::info(sCmd, "Registered");
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
        WSJCppLog::err(sCmd, "Not found");
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

void WJSCppCmdHandlerServerApi::handle(ModelRequest *pRequest) {
    nlohmann::json jsonResponse;
    jsonResponse["version"] = FHQSRV_VERSION; // TODO redesign, what?

    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();
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
        std::vector<CmdInputDef> ins = pHandler->inputs();
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
