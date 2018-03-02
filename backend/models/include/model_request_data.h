#ifndef MODEL_REQUEST_DATA_H
#define MODEL_REQUEST_DATA_H

#include <iserver.h>

class ModelRequestData {
    public:
		ModelRequestData(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject jsonData);
		QWebSocket *client();
		IWebSocketServer *server();
		QJsonObject data();
		std::string m();
		bool hasM();
		std::string command();
		bool hasCommand();

	private:
		QWebSocket *m_pClient;
		IWebSocketServer *m_pServer;
		QJsonObject m_jsonObject;
		std::string m_sMessageId;
		std::string m_sCommand;
};

#endif // MODEL_REQUEST_DATA_H
