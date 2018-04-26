#include <model_request.h>
#include <iostream>

// ---------------------------------------------------------------------

ModelRequest::ModelRequest(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject jsonData, nlohmann::json &jsonRequest_){
	m_pClient = pClient;
	m_pServer = pWebSocketServer;
    m_jsonObject = jsonData; // todo deprecated
    m_jsonRequest = jsonRequest_;

    if(m_jsonRequest["cmd"].is_string()){
        m_sCommand = m_jsonRequest["cmd"];
    }
	
    if(m_jsonRequest["m"].is_string()){
        m_sMessageId = m_jsonRequest["m"];
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

QJsonObject ModelRequest::data(){ // deprecated
	return m_jsonObject;
}

// ---------------------------------------------------------------------

const nlohmann::json& ModelRequest::jsonRequest(){
    return m_jsonRequest;
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

void ModelRequest::sendMessageSuccess(const std::string &cmd, nlohmann::json& jsonResponse){
    jsonResponse["cmd"] = cmd;
    jsonResponse["m"] = m_sMessageId;
    jsonResponse["result"] = "DONE";
    m_pServer->sendMessage(m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------

// deprecated
void ModelRequest::sendMessageSuccess(const std::string &cmd, QJsonObject jsonResponse){
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

// bool ModelRequest::validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler){
	
// }
