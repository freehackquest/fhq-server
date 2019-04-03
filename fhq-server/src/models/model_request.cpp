#include <model_request.h>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>

// ---------------------------------------------------------------------

ModelRequest::ModelRequest(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, nlohmann::json &jsonRequest_) {
    m_pClient = pClient;
    m_pServer = pWebSocketServer;
    m_jsonRequest = jsonRequest_;

    if(m_jsonRequest["cmd"].is_string()) {
        m_sCommand = m_jsonRequest["cmd"];
    }

    if(m_jsonRequest["m"].is_string()){
        m_sMessageId = m_jsonRequest["m"];
    }
}

// ---------------------------------------------------------------------

QWebSocket *ModelRequest::client() {
    return m_pClient;
}

// ---------------------------------------------------------------------

IWebSocketServer *ModelRequest::server() {
    return m_pServer;
}

// ---------------------------------------------------------------------

const nlohmann::json& ModelRequest::jsonRequest() {
    return m_jsonRequest;
}

// ---------------------------------------------------------------------

std::string ModelRequest::m(){
    return m_sMessageId;
}

// ---------------------------------------------------------------------

IUserToken *ModelRequest::userToken() {
    return m_pServer->getUserToken(m_pClient);
}

// ---------------------------------------------------------------------

QJsonObject ModelRequest::data() { // deprecated
    QString s = QString::fromStdString( m_jsonRequest.dump() );
    return QJsonDocument::fromJson(s.toUtf8()).object();
}

// ---------------------------------------------------------------------

void ModelRequest::sendMessageError(const std::string &cmd, Error error) {
    m_pServer->sendMessageError(m_pClient,cmd,m_sMessageId,error);
}

// ---------------------------------------------------------------------

void ModelRequest::sendMessageSuccess(const std::string &cmd, nlohmann::json& jsonResponse) {
    jsonResponse["cmd"] = cmd;
    jsonResponse["m"] = m_sMessageId;
    jsonResponse["result"] = "DONE";
    m_pServer->sendMessage(m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------

bool ModelRequest::hasM(){
    return m_sMessageId != "";
}

// ---------------------------------------------------------------------

std::string ModelRequest::command() {
    return m_sCommand;
}

// ---------------------------------------------------------------------

bool ModelRequest::hasCommand() {
    return m_sCommand != "";
}

// ---------------------------------------------------------------------

// bool ModelRequest::validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler){

// }
