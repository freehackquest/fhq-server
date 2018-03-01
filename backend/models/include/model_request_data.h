#ifndef MODEL_REQUEST_DATA_H
#define MODEL_REQUEST_DATA_H

#include <iserver.h>

class ModelRequestData {
    public:
		ModelRequestData(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, std::string m, QJsonObject obj);
		QWebSocket *client();
		IWebSocketServer *server();
		QJsonObject data();
		std::string m();

	private:
		QWebSocket *m_pClient;
		IWebSocketServer *m_pServer;
		std::string m_sMessage;
		QJsonObject m_jsonObject;
};

#endif // MODEL_REQUEST_DATA_H
