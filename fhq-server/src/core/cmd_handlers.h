#ifndef CMD_HADNLERS_H
#define CMD_HADNLERS_H

#include <map>
#include <fallen.h>
#include <wsjcpp_validators.h>
#include <json.hpp>
#include <QWebSocket>
#include <QSqlQuery> // TODO deprecated
#include <QSqlRecord> // TODO deprecated
#include <QString> // TODO deprecated
#include <QVariant> // TODO deprecated

/*! 
 * WsjcppJsonRpc20Error - helper class for errors
 * */

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
 * WsjcppJsonRpc20UserSession - 
 * WsjcppJsonRpc20UserSession - user data session
 * */

class WsjcppJsonRpc20UserSession {
    public:
        WsjcppJsonRpc20UserSession();
        WsjcppJsonRpc20UserSession(nlohmann::json const& obj);
        void fillFrom(nlohmann::json const& obj);

        std::string getSessionUuid();
        void setSessionUuid(const std::string &sSessionUuid);

        long getSessionCreated();
        void setSessionCreated(long nSessionCreated);

        long getSessionUpdated();
        void setSessionUpdated(long nSessionUpdated);

        long getSessionExpire();
        void setSessionExpire(long nSessionExpire);

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

        // IUserToken
        bool isAdmin();
        bool isUser();
        bool isTester();
        bool hasRole();
        
        nlohmann::json getUserCustom();
        void setUserCustom(const nlohmann::json &jsonCustom);

    private:
        std::string TAG;
        std::string m_sSessionUuid;
        long m_nSessionCreated;
        long m_nSessionUpdated;
        long m_nSessionExpire;
        int m_nUserID;
        std::string m_sUserUuid;
        std::string m_sUserName;
        std::string m_sUserEmail;
        std::string m_sUserRole;
        nlohmann::json m_jsonUserCustom;
};

/*! 
 * WsjcppSocketClient - 
 * */

class WsjcppSocketClient : public QObject {
    private:
        Q_OBJECT
        
    public:
        WsjcppSocketClient(QWebSocket *pSocket);
        ~WsjcppSocketClient();
        
    private:
        std::string TAG;
        WsjcppJsonRpc20UserSession *m_pUserSession;
        QWebSocket *m_pSocket;

    private Q_SLOTS:
        void processTextMessage(const QString &message);
        void processBinaryMessage(QByteArray message);
        void socketDisconnected();
};

/*! 
 * IWebSocketServer - 
 * */

class IWebSocketServer {
    public:
        virtual void sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse) = 0;
        virtual void sendMessageError(QWebSocket *pClient, const std::string &sCmd, const std::string & sM, WsjcppJsonRpc20Error error) = 0;
        virtual void sendToAll(const nlohmann::json& jsonMessage) = 0;
        virtual void sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) = 0;
        virtual int getConnectedUsers() = 0;
        virtual void setWsjcppJsonRpc20UserSession(QWebSocket *pClient, WsjcppJsonRpc20UserSession *pUserSession) = 0; 
        virtual WsjcppJsonRpc20UserSession *getWsjcppJsonRpc20UserSession(QWebSocket *pClient) = 0;
};

/*! 
 * CmdInputDef - helper api for define input params and descrip it for docs.
 * */
    
class CmdInputDef {
    public:
        CmdInputDef(const std::string &sName, const std::string &sDescription);
        CmdInputDef();
        CmdInputDef & optional();
        CmdInputDef & required();
        CmdInputDef & string_();
        CmdInputDef & integer_();
        CmdInputDef & any_();
        CmdInputDef & bool_();
        CmdInputDef & description(const std::string &sDescription);
        CmdInputDef & minval(int minval);
        CmdInputDef & maxval(int maxval);
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
        bool isInteger();
        bool isString();
        bool isBool();
        bool isAny();

        bool isMinVal(); // TODO: redesign to validators
        int getMinVal(); // TODO: redesign to validators
        bool isMaxVal(); // TODO: redesign to validators
        int getMaxVal(); // TODO: redesign to validators

        CmdInputDef &addValidator(WsjcppValidatorStringBase *pValidatorStringBase);
        
        const std::vector<WsjcppValidatorStringBase *> &listOfValidators();

    private:
        std::string m_sType;
        std::string m_sName;
        std::string m_sRestrict;
        std::string m_sDescription;
        int m_nMinVal;
        bool m_bSettedMinVal;
        int m_nMaxVal;
        bool m_bSettedMaxVal;

        std::string CMD_INPUT_DEF_TYPE_INTEGER = "integer";
        std::string CMD_INPUT_DEF_TYPE_STRING = "string";
        std::string CMD_INPUT_DEF_TYPE_BOOL = "boolean";
        std::string CMD_INPUT_DEF_TYPE_ANY = "any";

        std::vector<WsjcppValidatorStringBase *> m_vValidatorsString;
};

// ---------------------------------------------------------------------

class ModelRequest {
    public:
        ModelRequest(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, nlohmann::json &jsonRequest_);
        QWebSocket *client();
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
        QWebSocket *m_pClient;
        IWebSocketServer *m_pServer;
        WsjcppJsonRpc20UserSession *m_pWsjcppJsonRpc20UserSession;
        nlohmann::json m_jsonRequest;
        std::string m_sMessageId;
        std::string m_sCommand;
};

// ---------------------------------------------------------------------

/*!
 * Api handler Base
 * */

class CmdHandlerBase { // TODO rename to WJSCppHandler

    public:
        CmdHandlerBase(const std::string &sCmd, const std::string &sDescription);
        virtual std::string cmd();
        virtual std::string description();
        std::string activatedFromVersion();
        std::string deprecatedFromVersion();
        bool accessUnauthorized();
        bool accessUser();
        bool accessAdmin();
        bool checkAccess(ModelRequest *pRequest);

        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest) = 0;

        // virtual void success(nlohmann::json jsonResponse);
        // virtual void error(int nCode, const std::string &sErrorMessage);
        
    protected:
        void setAccessUnauthorized(bool bAccess);
        void setAccessUser(bool bAccess);
        void setAccessAdmin(bool bAccess);
        void setActivatedFromVersion(const std::string &sActivatedFromVersion);
        void setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion);

        CmdInputDef &requireStringParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalStringParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &requireIntegerParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalIntegerParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &requireBooleanParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalBooleanParam(const std::string &sName, const std::string &sDescription);

        std::string TAG;
        std::string m_sCmd;
        std::string m_sDescription;

    private:
        std::vector<CmdInputDef> m_vInputs; // TODO redesign to map
        // std::map<std::string, CmdInputDef*> *m_vCmdInputDefs;
        std::string m_sActivatedFromVersion;
        std::string m_sDeprecatedFromVersion;
        bool m_bAccessUnauthorized;
        bool m_bAccessUser;
        bool m_bAccessAdmin;
};

extern std::map<std::string, CmdHandlerBase*> *g_pCmdHandlers;

// ---------------------------------------------------------------------

/*!
 * Global collection with handlers
 * */

class CmdHandlers {
    public:
        static void initGlobalVariables();
        static void addHandler(const std::string &sName, CmdHandlerBase* pCmdHandler);
        static CmdHandlerBase *findCmdHandler(const std::string &sCmd);
};

// RegistryCmdHandler
#define REGISTRY_CMD( classname ) \
    static classname * pRegistry ## classname = new classname(); \

// ---------------------------------------------------------------------

/*!
 * This handler will be return list of handlers - publish api interfaces
 * */

class WJSCppCmdHandlerServerApi : public CmdHandlerBase {

    public:
        WJSCppCmdHandlerServerApi();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HADNLERS_H
