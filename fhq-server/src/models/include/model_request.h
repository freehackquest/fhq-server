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
        const nlohmann::json& jsonRequest();
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
        nlohmann::json m_jsonRequest;
        std::string m_sMessageId;
        std::string m_sCommand;
};

#endif // MODEL_REQUEST_H
