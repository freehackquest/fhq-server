#ifndef MODEL_REQUEST_DATA_H
#define MODEL_REQUEST_DATA_H

#include <iwebsocketserver.h>

class ModelRequest {
    public:
        ModelRequest(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject jsonData);
		QWebSocket *client();
		IWebSocketServer *server();
        IUserToken *userToken();
		QJsonObject data();
		std::string m();
		bool hasM();
		std::string command();
		bool hasCommand();
        void sendMessageError(const std::string &cmd, Error error);
        void sendMessageSuccess(std::string cmd, QJsonObject jsonResponse);

	private:
		QWebSocket *m_pClient;
		IWebSocketServer *m_pServer;
		QJsonObject m_jsonObject;
		std::string m_sMessageId;
		std::string m_sCommand;
};

#endif // MODEL_REQUEST_DATA_H
