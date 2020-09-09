#ifndef CMD_HADNLERS_H
#define CMD_HADNLERS_H

#include <map>
#include <fallen.h>
#include <wsjcpp_jsonrpc20.h>
#include <QWebSocket>
#include <QSqlQuery> // TODO deprecated
#include <QSqlRecord> // TODO deprecated
#include <QString> // TODO deprecated
#include <QVariant> // TODO deprecated

// ---------------------------------------------------------------------
// WsjcppJsonRpc20WebSocketServer
 
class WsjcppJsonRpc20WebSocketServer {
    public:
        virtual void sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse) = 0;
        virtual void sendMessageError(WsjcppJsonRpc20WebSocketClient *pClient, const std::string &sCmd, const std::string & sM, WsjcppJsonRpc20Error error) = 0;
        virtual void sendToAll(const nlohmann::json& jsonMessage) = 0;
        virtual void sendToOne(WsjcppJsonRpc20WebSocketClient *pClient, const nlohmann::json &jsonMessage) = 0;
        virtual int getConnectedUsers() = 0;
        virtual void setUserSession(void *pClient, WsjcppJsonRpc20UserSession *pUserSession) = 0;
        virtual void unsetUserSession(void *pClient) = 0; 
        virtual WsjcppJsonRpc20UserSession *findUserSession(void *pClient) = 0;
};

// ---------------------------------------------------------------------

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
        // TODO set input defs
        
        const nlohmann::json& jsonRequest(); // TODO deprecated
        bool hasInputParam(const std::string &sParamName);
        std::string getInputString(const std::string &sParamName, const std::string &sDefaultValue);
        int getInputInteger(const std::string &sParamName, int defaultValue);
        
        std::string getId();
        std::string getMethod();

        void sendMessageSuccess(const std::string &cmd, nlohmann::json& jsonResponseResult); // TODO deprecated
        void sendResponse(nlohmann::json& jsonResult); // TODO deprecated

        // TODO done()
        void fail(WsjcppJsonRpc20Error error);
        // bool validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler);
    private:
        std::string TAG;
        WsjcppJsonRpc20WebSocketClient *m_pClient;
        WsjcppJsonRpc20WebSocketServer *m_pServer;
        WsjcppJsonRpc20UserSession *m_pWsjcppJsonRpc20UserSession;
        nlohmann::json m_jsonRequest;
        std::string m_sId;
        std::string m_sMethod;
        bool m_bResponseSend;
};

// ---------------------------------------------------------------------

/*!
 * Api handler Base
 * */

class CmdHandlerBase {

    public:
        CmdHandlerBase(const std::string &sCmd, const std::string &sDescription);
        virtual std::string cmd();
        virtual std::string getDescription() const;
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

        virtual const std::vector<WsjcppJsonRpc20ParamDef> &inputs();
        virtual void handle(WsjcppJsonRpc20Request *pRequest) = 0;

        // virtual void success(nlohmann::json jsonResponse);
        // virtual void error(int nCode, const std::string &sErrorMessage);
        
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
        bool m_bAccessTester;
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
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

#endif // CMD_HADNLERS_H
