#include "wsjcpp_jsonrpc20.h"
#include <wsjcpp_core.h>

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
    // {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request."}, "id": null}
}

// ---------------------------------------------------------------------

/*! 
 * WsjcppJsonRpc20UserSession - all data by current user session
 * */

WsjcppJsonRpc20UserSession::WsjcppJsonRpc20UserSession() {
    TAG = "WsjcppJsonRpc20UserSession";
    m_nUserID = 0;
    m_sUserRole = "";
    m_sUserEmail = "";
    m_sUserUuid = "";
    m_sUserName = "";
    m_sSessionUuid = "";
    m_nSessionCreated = 0;
    m_nSessionUpdated = 0;
    m_nSessionExpireAt = 0;
    m_jsonSessionCustom = nlohmann::json{};
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20UserSession::WsjcppJsonRpc20UserSession(nlohmann::json const& obj) : WsjcppJsonRpc20UserSession() {
    this->fillFrom(obj);
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::fillFrom(const nlohmann::json &obj) {
    if (obj.find("uuid") != obj.end()) {
        try {
            m_sSessionUuid = obj.at("uuid").get<std::string>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "Something wrong param session.uuid in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
            m_sSessionUuid = "";
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'uuid' in struct");
        m_sSessionUuid = "";
    }

    if (obj.find("created") != obj.end()) {
        try {
            m_nSessionCreated = obj.at("created").get<long>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "Something wrong param session.created in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
            m_nSessionCreated = 0;
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'created' in struct");
        m_nSessionCreated = 0;
    }

    if (obj.find("updated") != obj.end()) {
        try {
            m_nSessionUpdated = obj.at("updated").get<long>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "Something wrong param session.updated in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
            m_nSessionUpdated = 0;
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'updated' in struct");
        m_nSessionUpdated = 0;
    }

    if (obj.find("expire_at") != obj.end()) {
        try {
            m_nSessionExpireAt = obj.at("expire_at").get<long>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "Something wrong param session.expire_at in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
            m_nSessionExpireAt = 0;
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'expire_at' in struct");
        m_nSessionExpireAt = 0;
    }

    if (obj.find("custom") != obj.end()) {
        m_jsonSessionCustom = obj["custom"];
    } else {
        WsjcppLog::warn(TAG, "Not found param 'custom' in struct");
        m_jsonSessionCustom = nlohmann::json{};
    }

    if (obj.find("user") != obj.end()) {
        nlohmann::json user = obj.at("user");

        if (user.find("role") != user.end()) {
            try {
                m_sUserRole = user.at("role").get<std::string>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.role in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
                m_sUserRole = "";
            }
        } else {
            m_sUserRole = "";
        }

        if (user.find("id") != user.end()) {
            try {
                m_nUserID = user.at("id").get<int>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.id in struct: "
                    + obj.dump() + "; error: " + std::string(e.what()));
                m_nUserID = 0;
            }
        } else {
            m_nUserID = 0;
        }
        
        // user.email
        if (user.find("email") != user.end()) {
            try {
                m_sUserEmail = user.at("email").get<std::string>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.email in struct: "
                    + obj.dump() + "; error: " + std::string(e.what()));
                m_sUserEmail = "";
            }
        } else {
            m_sUserEmail = "";
        }

        // user.nick
        if (user.find("name") != user.end()) {
            try {
                m_sUserName = user.at("name").get<std::string>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.name in struct: "
                    + obj.dump() + "; error: " + std::string(e.what()));
                m_sUserName = "";
            }
        } else {
            m_sUserName = "";
        }

        // user.uuid
        if (user.find("uuid") != user.end()) {
            try {
                m_sUserUuid = user.at("uuid").get<std::string>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.uuid in struct: "
                    + obj.dump() + "; error: " + std::string(e.what()));
                m_sUserUuid = "";
            }
        } else {
            m_sUserUuid = "";
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'user' in struct");
        m_nUserID = 0;
        m_sUserRole = "";
        m_sUserName = "";
        m_sUserUuid = "";
        m_sUserEmail = "";
    }
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20UserSession::toJson() {
    nlohmann::json jsonRet;
    jsonRet["uuid"] = m_sSessionUuid;
    jsonRet["created"] = m_nSessionCreated;
    jsonRet["updated"] = m_nSessionUpdated;
    jsonRet["expire_at"] = m_nSessionExpireAt;

    nlohmann::json jsonUser;
    jsonUser["id"] = m_nUserID;
    jsonUser["uuid"] = m_sUserUuid;
    jsonUser["name"] = m_sUserName;
    jsonUser["email"] = m_sUserEmail;
    jsonUser["role"] = m_sUserRole;
    jsonRet["user"] = jsonUser;

    jsonRet["custom"] = m_jsonSessionCustom;

    return jsonRet;
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

long WsjcppJsonRpc20UserSession::getSessionExpireAt() {
    return m_nSessionExpireAt;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionExpireAt(long nSessionExpireAt) {
    m_nSessionExpireAt = nSessionExpireAt;
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

nlohmann::json WsjcppJsonRpc20UserSession::getSessionCustom() {
    return m_jsonSessionCustom;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionCustom(const nlohmann::json &jsonSessionCustom) {
    m_jsonSessionCustom = jsonSessionCustom;
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20ParamDef

WsjcppJsonRpc20ParamDef::WsjcppJsonRpc20ParamDef(const std::string &sName, const std::string &sDescription) 
    : WsjcppJsonRpc20ParamDef()
{
    m_sName = sName;
    m_sDescription = sDescription;
    
    if (sName.length() < 1) {
        WsjcppLog::throw_err(TAG, "Name must be define");
    }
    for (int i = 0; i < sName.length(); i++) {
        char c =  sName[i];
        if ((c < 'a' || c > 'z') && (c < '0' || c > '9') && c != '_') {
            std::string sError = "Not allowed character in name of param: '";
            sError += c;
            sError += "', character must be in range 'a-z0-9_'";
            WsjcppLog::throw_err(TAG, sError);
        }
        
    }
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef::WsjcppJsonRpc20ParamDef() {
    TAG = "WsjcppJsonRpc20ParamDef";
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
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Type already defined like '" + m_sType + "', but you want change this to 'string'");
    }
    m_sType = std::string(JSONRPC20_PARAM_DEF_TYPE_STRING);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::integer_() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Type already defined like '" + m_sType + "', but you want change this to 'integer'");
    }
    m_sType = JSONRPC20_PARAM_DEF_TYPE_INTEGER;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::bool_() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Type already defined like '" + m_sType + "', but you want change this to 'bool'");
    }
    m_sType = std::string(JSONRPC20_PARAM_DEF_TYPE_BOOL);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::json_() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Type already defined like '" + m_sType + "', but you want change this to 'json'");
    }
    m_sType = std::string(JSONRPC20_PARAM_DEF_TYPE_JSON);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::description(const std::string& s) {
    m_sDescription = s;
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

bool WsjcppJsonRpc20ParamDef::isOptional() {
    return m_sRestrict == "optional";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isInteger() {
    return m_sType == JSONRPC20_PARAM_DEF_TYPE_INTEGER;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isString() {
    return m_sType == JSONRPC20_PARAM_DEF_TYPE_STRING;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isBool() {
    return m_sType == JSONRPC20_PARAM_DEF_TYPE_BOOL;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isJson() {
    return m_sType == JSONRPC20_PARAM_DEF_TYPE_JSON;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20ParamDef::addValidator(WsjcppValidatorStringBase *pValidatorString) {
    if (!isString()) {
        WsjcppLog::throw_err(TAG, "addValidator(), expected string data type");
    }
    m_vValidatorsString.push_back(pValidatorString);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20ParamDef::addValidator(WsjcppValidatorIntegerBase *pValidatorInteger) {
    if (!isInteger()) {
        WsjcppLog::throw_err(TAG, "addValidator(), expected integer data type");
    }
    m_vValidatorsInteger.push_back(pValidatorInteger);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20ParamDef::addValidator(WsjcppValidatorJsonBase *pValidatorJson) {
    if (!isJson()) {
        WsjcppLog::throw_err(TAG, "addValidator(), expected json data type");
    }
    m_vValidatorsJson.push_back(pValidatorJson);
    return *this;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppValidatorStringBase *> &WsjcppJsonRpc20ParamDef::listOfStringValidators() {
    return m_vValidatorsString;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppValidatorIntegerBase *> &WsjcppJsonRpc20ParamDef::listOfIntegerValidators() {
    return m_vValidatorsInteger;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppValidatorJsonBase *> &WsjcppJsonRpc20ParamDef::listOfJsonValidators() {
    return m_vValidatorsJson;
}

// ---------------------------------------------------------------------