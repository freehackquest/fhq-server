#ifndef WSJCPP_JSONRPC20
#define WSJCPP_JSONRPC20

#include <json.hpp>
#include <wsjcpp_validators.h>
#include <map>
#include <iostream>
#include <algorithm>

/*! 
 * WsjcppJsonRpc20Error - helper class for errors
 * */

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

/*! 
 * WsjcppJsonRpc20Session - user data session
 * */

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

/*! 
 * IWebSocketServer - 
 * */

/*class IWebSocketServer {
    public:
        // virtual void sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse) = 0;
        // virtual void sendMessageError(QWebSocket *pClient, const std::string &sCmd, const std::string & sM, WsjcppError error) = 0;
        virtual void sendToAll(const nlohmann::json& jsonMessage) = 0;
        // virtual void sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) = 0;
        virtual int getConnectedUsers() = 0;
        // virtual void setWsjcppJsonRpc20UserSession(QWebSocket *pClient, WsjcppJsonRpc20UserSession *pUserSession) = 0; 
        // virtual WsjcppJsonRpc20UserSession *getWsjcppJsonRpc20UserSession(QWebSocket *pClient) = 0;
};*/

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

/*
class WsjcppJsonRpc20Request {
    public:
        WsjcppJsonRpc20Request(void *pClient, IWebSocketServer *pWebSocketServer, nlohmann::json &jsonRequest_);
        void *client();
        std::string getIpAddress();
        IWebSocketServer *server();
        WsjcppJsonRpc20UserSession *getUserSession();
        bool isAdmin();
        bool isUser();
        bool isUnauthorized();
        // TODO set input defs
        
        const nlohmann::json& jsonRequest(); // TODO deprecated
        bool hasInputParam(const std::string &sParamName);
        std::string getInputString(const std::string &sParamName, const std::string &sDefaultValue);
        int getInputInteger(const std::string &sParamName, int defaultValue);
        
        std::string m();
        bool hasM();
        std::string command();
        bool hasCommand();
        void sendMessageError(const std::string &cmd, WsjcppJsonRpc20Error error);
        void sendMessageSuccess(const std::string &cmd, nlohmann::json& jsonResponse);
        void sendResponse(nlohmann::json& jsonResult);

        // bool validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler);
    private:
        std::string TAG;
        void *m_pClient;
        IWebSocketServer *m_pServer;
        WsjcppJsonRpc20UserSession *m_pWsjcppJsonRpc20UserSession;
        nlohmann::json m_jsonRequest;
        std::string m_sMessageId;
        std::string m_sCommand;
};
*/
// ---------------------------------------------------------------------

/*!
 * Api handler Base
 * */

/*
class WsjcppJsonRpc20Base {

    public:
        WsjcppJsonRpc20Base(const std::string &sMethod, const std::string &sDescription);
        virtual std::string getMethodName() const;
        virtual std::string getDescription() const;
        std::string getActivatedFromVersion() const;
        std::string getDeprecatedFromVersion() const;
        bool getAccessUnauthorized() const;
        bool getAccessUser() const;
        bool getAccessTester() const;
        bool getAccessAdmin() const;
        bool checkAccess(const WsjcppJsonRpc20Request *pRequest) const;

        virtual const std::vector<WsjcppJsonRpc20ParamDef> &inputs();
        virtual void handle(WsjcppJsonRpc20Request *pRequest) = 0;

        // virtual void done(nlohmann::json jsonResponse);
        // virtual void fail(int nCode, const std::string &sErrorMessage);
        
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
        std::string m_sCmd;
        std::string m_sDescription;

    private:
        std::vector<WsjcppJsonRpc20ParamDef> m_vInputs; // TODO redesign to map
        // std::map<std::string, WsjcppJsonRpc20ParamDef*> *m_vWsjcppJsonRpc20ParamDefs;
        std::string m_sActivatedFromVersion;
        std::string m_sDeprecatedFromVersion;
        bool m_bAccessUnauthorized;
        bool m_bAccessUser;
        bool m_bAccessAdmin;
};
*/
//extern std::map<std::string, WsjcppJsonRpc20Base*> *g_pWsjcppJsonRpc20BaseList;

/*!
 * Global collection with handlers
 * */
/*
class WsjcppJsonRpc20 {
    public:
        static void initGlobalVariables();
        static void addHandler(const std::string &sName, WsjcppJsonRpc20Base* pCmdHandler);
        static WsjcppJsonRpc20Base *findJsonRpc20(const std::string &sCmd);
};

// RegistryWsjcppJsonRpc20
#define REGISTRY_WSJCPP_JSONRPC20( classname ) \
    static classname * pRegistryWsjcppJsonRpc20 ## classname = new classname(); \
*/
// ---------------------------------------------------------------------

/*!
 * This handler will be return list of handlers - publish api interfaces
 * */
/*
class WsjcppJsonRpc20ServerApi : public WsjcppJsonRpc20Base {

    public:
        WsjcppJsonRpc20ServerApi();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};
*/
#endif // WSJCPP_JSONRPC20