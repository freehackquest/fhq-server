#ifndef MODEL_REQUEST_H
#define MODEL_REQUEST_H

#include <iwebsocketserver.h>
#include <json.hpp>

class ModelRequest {
    public:
        ModelRequest(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, nlohmann::json &jsonRequest_);
        QWebSocket *client();
        IWebSocketServer *server();
        IUserToken *userToken();
        bool isAdmin();
        bool isUser();
        bool isUnauthorized();
        // TODO set input defs
        QJsonObject data(); // TODO deprecated
        const nlohmann::json& jsonRequest(); // TODO deprecated
        std::string getInputString(const std::string &sParamName, const std::string &sDefaultValue);
        int getInputInteger(const std::string &sParamName, int defaultValue);
        
        std::string m();
        bool hasM();
        std::string command();
        bool hasCommand();
        void sendMessageError(const std::string &cmd, Error error);
        void sendMessageSuccess(const std::string &cmd, nlohmann::json& jsonResponse);

        // bool validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler);
    private:
        QWebSocket *m_pClient;
        IWebSocketServer *m_pServer;
        IUserToken *m_pUserToken;
        nlohmann::json m_jsonRequest;
        std::string m_sMessageId;
        std::string m_sCommand;
};

#endif // MODEL_REQUEST_H
