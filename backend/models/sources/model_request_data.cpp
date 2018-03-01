#include <model_request_data.h>

// ---------------------------------------------------------------------

ModelRequestData::ModelRequestData(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject jsonData){
	m_pClient = pClient;
	m_pServer = pWebSocketServer;
	m_jsonObject = jsonData;
	
	if(m_jsonObject.contains("cmd")){
		m_sCommand = m_jsonObject["cmd"].toString().toStdString();
	}
	
	if(m_jsonObject.contains("m")){
		m_sMessageId = m_jsonObject["m"].toString().toStdString();
	}
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
	return m_sMessageId;
}

// ---------------------------------------------------------------------

bool ModelRequestData::hasM(){
	return m_sMessageId != "";
}

// ---------------------------------------------------------------------

std::string ModelRequestData::command(){
	return m_sCommand;
}

// ---------------------------------------------------------------------

bool ModelRequestData::hasCommand(){
	return m_sCommand != "";
}

// ---------------------------------------------------------------------
