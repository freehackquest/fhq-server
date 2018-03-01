#include <model_request_data.h>

// ---------------------------------------------------------------------

ModelRequestData::ModelRequestData(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, std::string m, QJsonObject obj){
	m_pClient = pClient;
	m_pServer = pWebSocketServer;
	m_sMessage = m;
	m_jsonObject = obj;
}

// ---------------------------------------------------------------------

QWebSocket *ModelRequestData::client(){
	return m_pClient;
}

// ---------------------------------------------------------------------

IWebSocketServer *ModelRequestData::server(){
	return m_pServer;
}

// ---------------------------------------------------------------------

QJsonObject ModelRequestData::data(){
	return m_jsonObject;
}

// ---------------------------------------------------------------------

std::string ModelRequestData::m(){
	return m_sMessage;
}

// ---------------------------------------------------------------------
