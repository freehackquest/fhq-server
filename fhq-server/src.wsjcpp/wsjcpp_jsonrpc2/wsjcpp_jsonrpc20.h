#ifndef WSJCPP_JSONRPC20
#define WSJCPP_JSONRPC20

#include <json.hpp>
#include <wsjcpp_validators.h>
#include <map>
#include <iostream>
#include <algorithm>

class WsjcppJsonRpc20Error;
class WsjcppJsonRpc20UserSession;
class WsjcppJsonRpc20ParamDef;
class WsjcppJsonRpc20WebSocketClient;
class WsjcppJsonRpc20WebSocketServer;
class WsjcppJsonRpc20ParamDef;
class WsjcppJsonRpc20Request;
class WsjcppJsonRpc20BaseHandler;
class WsjcppJsonRpc20;

// ---------------------------------------------------------------------
// WsjcppJsonRpc20Error

// must be this json:
// {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request."}, "id": null}
class WsjcppJsonRpc20Error {
    public:
        WsjcppJsonRpc20Error(int nErrorCode, const std::string &sErrorMessage);
        int getErrorCode();
        std::string getErrorMessage();
        nlohmann::json toJson();
    private:
        std::string m_sErrorMessage;
        int m_nErrorCode;
};

// ---------------------------------------------------------------------
// WsjcppJsonRpc20Session

class WsjcppJsonRpc20UserSession {
    public:
        WsjcppJsonRpc20UserSession();
        WsjcppJsonRpc20UserSession(nlohmann::json const& obj);
        void fillFrom(nlohmann::json const& obj);
        nlohmann::json toJson();

        std::string getSessionUuid();
        void setSessionUuid(const std::string& sSessionUuid);

        long getSessionCreated();
        void setSessionCreated(long nSessionCreated);

        long getSessionUpdated();
        void setSessionUpdated(long nSessionUpdated);

        long getSessionExpireAt();
        void setSessionExpireAt(long nSessionExpire);

        int getUserId();
        void setUserId(int nId);
        
        std::string getUserUuid();
        void setUserUuid(const std::string &sUserUuid);

        std::string getUserName();
        void setUserName(const std::string &sUserName);

        std::string getUserEmail();
        void setUserEmail(const std::string &sUserEmail);

        std::string getUserRole();
        void setUserRole(const std::string &sUserRole);

        bool isAdmin();
        bool isUser();
        bool isTester();
        bool hasRole();

        nlohmann::json getSessionCustom();
        void setSessionCustom(const nlohmann::json &jsonSessionCustom);

    private:
        std::string TAG;
        std::string m_sSessionUuid;
        long m_nSessionCreated;
        long m_nSessionUpdated;
        long m_nSessionExpireAt;

        int m_nUserID;
        std::string m_sUserUuid;
        std::string m_sUserName;
        std::string m_sUserEmail;
        std::string m_sUserRole;
        nlohmann::json m_jsonSessionCustom;
};

// ---------------------------------------------------------------------
// WsjcppJsonRpc20WebSocketClient

class WsjcppJsonRpc20WebSocketClient {
    public:
        WsjcppJsonRpc20WebSocketClient();
        void setUserSession(WsjcppJsonRpc20UserSession *pUserSession);
        WsjcppJsonRpc20UserSession *getUserSession();
        void unsetUserSession();
        virtual void onDisconnected() = 0;
        virtual std::string getPeerIpAddress() = 0;
        virtual int getPeerPort() = 0;
        virtual std::string getRequestUrl() = 0;

        virtual void sendTextMessage(const std::string &sTextMessage) = 0;

    protected:
        std::string TAG;

    private:
        WsjcppJsonRpc20UserSession *m_pUserSession;
};

// ---------------------------------------------------------------------
// WsjcppJsonRpc20WebSocketServer

class WsjcppJsonRpc20WebSocketServer {
    public:
        WsjcppJsonRpc20WebSocketServer();


        void onWebSocketConnected(void *pClient, WsjcppJsonRpc20WebSocketClient *pWebSocketClient);
        void onWebSocketDisconnected(void *pClient);
        int getConnectedClients();
        // TODO int getConnectedUsers();

        WsjcppJsonRpc20WebSocketClient *findWebSocketClient(void *pClient);
        void sendMessageToAll(const nlohmann::json& jsonMessage);
        void sendMessageToOne(WsjcppJsonRpc20WebSocketClient *pClient, const nlohmann::json &jsonMessage);

    protected:
        std::string TAG;

    private:
        std::map<void *, WsjcppJsonRpc20WebSocketClient *> m_mapClients;
};

/*! 
 * WsjcppJsonRpc20ParamDef - helper api for define input params and descrip it for docs.
 * */
// {"jsonrpc": "2.0", "method": "subtract", "params": {"subtrahend": 23, "minuend": 42}, "id": 3}

class WsjcppJsonRpc20ParamDef {
    public:
        WsjcppJsonRpc20ParamDef(const std::string &sName, const std::string &sDescription);
        WsjcppJsonRpc20ParamDef();
        WsjcppJsonRpc20ParamDef & optional();
        WsjcppJsonRpc20ParamDef & required();
        WsjcppJsonRpc20ParamDef & string_();
        WsjcppJsonRpc20ParamDef & integer_();
        WsjcppJsonRpc20ParamDef & json_();
        WsjcppJsonRpc20ParamDef & bool_();
        WsjcppJsonRpc20ParamDef & description(const std::string &sDescription);
        nlohmann::json toJson();
        
        const std::string &getType();
        const std::string &getType() const;
        const std::string &getName();
        const std::string &getName() const;
        const std::string &getRestrict();
        const std::string &getRestrict() const;
        const std::string &getDescription();
        const std::string &getDescription() const;

        bool isRequired();
        bool isOptional();
        bool isInteger();
        bool isString();
        bool isBool();
        bool isJson();

        WsjcppJsonRpc20ParamDef &addValidator(WsjcppValidatorStringBase *pValidatorString);
        WsjcppJsonRpc20ParamDef &addValidator(WsjcppValidatorIntegerBase *pValidatorInteger);
        WsjcppJsonRpc20ParamDef &addValidator(WsjcppValidatorJsonBase *pValidatorJson);
        
        const std::vector<WsjcppValidatorStringBase *> &listOfStringValidators();
        const std::vector<WsjcppValidatorIntegerBase *> &listOfIntegerValidators();
        const std::vector<WsjcppValidatorJsonBase *> &listOfJsonValidators();

    private:
        std::string TAG;
        std::string m_sType;
        std::string m_sName;
        std::string m_sRestrict;
        std::string m_sDescription;

        std::string JSONRPC20_PARAM_DEF_TYPE_INTEGER = "integer";
        std::string JSONRPC20_PARAM_DEF_TYPE_STRING = "string";
        std::string JSONRPC20_PARAM_DEF_TYPE_BOOL = "boolean";
        std::string JSONRPC20_PARAM_DEF_TYPE_JSON = "json";

        std::vector<WsjcppValidatorStringBase *> m_vValidatorsString;
        std::vector<WsjcppValidatorIntegerBase *> m_vValidatorsInteger;
        std::vector<WsjcppValidatorJsonBase *> m_vValidatorsJson;
};

// ---------------------------------------------------------------------
// WsjcppJsonRpc20Request

class WsjcppJsonRpc20Request {
    public:
        WsjcppJsonRpc20Request(
            WsjcppJsonRpc20WebSocketClient *pClient,
            WsjcppJsonRpc20WebSocketServer *pWebSocketServer
        );

        bool parseIncomeData(const std::string &sIncomeData);

        WsjcppJsonRpc20WebSocketClient *getWebSocketClient();
        WsjcppJsonRpc20WebSocketServer *getServer();
        WsjcppJsonRpc20UserSession *getUserSession();
        bool isAdmin();
        bool isUser();
        bool isUnauthorized();
        
        const nlohmann::json& jsonRequest(); // TODO deprecated
        bool hasInputParam(const std::string &sParamName);
        std::string getInputString(const std::string &sParamName, const std::string &sDefaultValue);
        int getInputInteger(const std::string &sParamName, int defaultValue);
        
        std::string getId();
        std::string getMethod();

        void done(nlohmann::json& jsonResponseResult);
        void fail(WsjcppJsonRpc20Error error);

    private:
        std::string TAG;
        WsjcppJsonRpc20WebSocketClient *m_pWebSocketClient;
        WsjcppJsonRpc20WebSocketServer *m_pServer;

        nlohmann::json m_jsonRequest;
        std::string m_sId;
        std::string m_sMethod;
        bool m_bResponseSend;
};

// ---------------------------------------------------------------------
// WsjcppJsonRpc20HandlerBase - api handler basic class

class WsjcppJsonRpc20HandlerBase {

    public:
        WsjcppJsonRpc20HandlerBase(const std::string &sMethodName, const std::string &sDescription);
        std::string getMethodName() const;
        std::string getDescription() const;
        std::string getActivatedFromVersion() const;
        std::string getDeprecatedFromVersion() const;
        bool haveUnauthorizedAccess() const;
        bool haveUserAccess() const;
        bool haveTesterAccess() const;
        bool haveAdminAccess() const;
        bool checkAccess(
            WsjcppJsonRpc20Request *pRequest,
            WsjcppJsonRpc20Error& error
        ) const;
        const std::vector<WsjcppJsonRpc20ParamDef> &inputs();

        virtual void handle(WsjcppJsonRpc20Request *pRequest) = 0;
        
    protected:
        void setAccessUnauthorized(bool bAccess);
        void setAccessUser(bool bAccess);
        void setAccessTester(bool bAccess);
        void setAccessAdmin(bool bAccess);
        void setActivatedFromVersion(const std::string &sActivatedFromVersion);
        void setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion);

        WsjcppJsonRpc20ParamDef &requireStringParam(const std::string &sName, const std::string &sDescription);
        WsjcppJsonRpc20ParamDef &optionalStringParam(const std::string &sName, const std::string &sDescription);
        WsjcppJsonRpc20ParamDef &requireIntegerParam(const std::string &sName, const std::string &sDescription);
        WsjcppJsonRpc20ParamDef &optionalIntegerParam(const std::string &sName, const std::string &sDescription);
        WsjcppJsonRpc20ParamDef &requireBooleanParam(const std::string &sName, const std::string &sDescription);
        WsjcppJsonRpc20ParamDef &optionalBooleanParam(const std::string &sName, const std::string &sDescription);

        std::string TAG;
        std::string m_sMethodName;
        std::string m_sDescription;

    private:
        std::vector<WsjcppJsonRpc20ParamDef> m_vInputs; // TODO redesign to map
        // std::map<std::string, WsjcppJsonRpc20ParamDef*> *m_vWsjcppJsonRpc20ParamDefs;
        std::string m_sActivatedFromVersion;
        std::string m_sDeprecatedFromVersion;
        bool m_bAccessUnauthorized;
        bool m_bAccessUser;
        bool m_bAccessTester;
        bool m_bAccessAdmin;
};


extern std::map<std::string, WsjcppJsonRpc20HandlerBase*> *g_pWsjcppJsonRpc20HandlerList;

// ---------------------------------------------------------------------
// Global collection with handlers

class WsjcppJsonRpc20 {
    public:
        static void initGlobalVariables();
        static void addHandler(const std::string &sName, WsjcppJsonRpc20HandlerBase* pCmdHandler);
        static WsjcppJsonRpc20HandlerBase *findJsonRpc20Handler(const std::string &sCmd);
};

// Registry Wsjcpp JsonRpc20 Handler
#define REGISTRY_WSJCPP_JSONRPC20_HANDLER( classname ) \
    static classname * pRegistryWsjcppJsonRpc20 ## classname = new classname(); \

// ---------------------------------------------------------------------
// This handler will be return list of handlers - publish api interfaces

class WsjcppJsonRpc20HandlerServerApi : public WsjcppJsonRpc20HandlerBase {

    public:
        WsjcppJsonRpc20HandlerServerApi();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

#endif // WSJCPP_JSONRPC20
