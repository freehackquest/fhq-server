#include <model_request.h>

// ---------------------------------------------------------------------

ModelRequest::ModelRequest(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject jsonData){
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

QWebSocket *ModelRequest::client(){
	return m_pClient;
}

// ---------------------------------------------------------------------

IWebSocketServer *ModelRequest::server(){
    return m_pServer;
}

// ---------------------------------------------------------------------

QJsonObject ModelRequest::data(){
	return m_jsonObject;
}

// ---------------------------------------------------------------------

std::string ModelRequest::m(){
	return m_sMessageId;
}

// ---------------------------------------------------------------------

IUserToken *ModelRequest::userToken(){
    return m_pServer->getUserToken(m_pClient);
}

// ---------------------------------------------------------------------

void ModelRequest::sendMessageError(const std::string &cmd, Error error){
    m_pServer->sendMessageError(m_pClient,cmd,QString(m_sMessageId.c_str()),error);
}

// ---------------------------------------------------------------------

void ModelRequest::sendMessageSuccess(std::string cmd, QJsonObject jsonResponse){
    jsonResponse["cmd"] = QJsonValue(QString(cmd.c_str()));
    jsonResponse["m"] = QJsonValue(QString(m_sMessageId.c_str()));
    jsonResponse["result"] = QJsonValue("DONE");
    m_pServer->sendMessage(m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------

bool ModelRequest::hasM(){
	return m_sMessageId != "";
}

// ---------------------------------------------------------------------

std::string ModelRequest::command(){
	return m_sCommand;
}

// ---------------------------------------------------------------------

bool ModelRequest::hasCommand(){
	return m_sCommand != "";
}

// ---------------------------------------------------------------------
