#include <websocketserver.h>

#include <QtCore> // TODO remove
#include <QDateTime>
#include <QRegularExpression>

#include <QHostAddress>
#include <QThread>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

#include <SmtpMime>
#include <utils_logger.h>

#include <employ_server_config.h>
#include <employ_server_info.h>
#include <employ_ws_server.h>
#include <model_request.h>
#include <cmd_handlers.h>


// QT_USE_NAMESPACE

// ---------------------------------------------------------------------

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent) {
    TAG = "WebSocketServer";

    m_bFailed = false;
    if(!Employees::init({"start_ws_server"})){
        m_bFailed = true;
        return;
    }

    EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();
    EmployWsServer *pWsServer = findEmploy<EmployWsServer>();

    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("freehackquest-backend"), QWebSocketServer::NonSecureMode, this);
    m_pWebSocketServerSSL = new QWebSocketServer(QStringLiteral("freehackquest-backend"), QWebSocketServer::SecureMode, this);

    if (m_pWebSocketServer->listen(QHostAddress::Any, pServerConfig->serverPort())) {
        Log::info(TAG, "fhq-server listening on port " + QString::number(pServerConfig->serverPort()));
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
    }else{
        Log::err(TAG, "fhq-server can not listening on port " + QString::number(pServerConfig->serverPort()));
        m_bFailed = true;
        return;
    }

    if(pServerConfig->serverSslOn()){
        QSslConfiguration sslConfiguration;
        QFile certFile(QString(pServerConfig->serverSslCertFile().c_str()));
        QFile keyFile(QString(pServerConfig->serverSslKeyFile().c_str()));
        certFile.open(QIODevice::ReadOnly);
        keyFile.open(QIODevice::ReadOnly);
        QSslCertificate certificate(&certFile, QSsl::Pem);
        QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
        certFile.close();
        keyFile.close();
        sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfiguration.setLocalCertificate(certificate);
        sslConfiguration.setPrivateKey(sslKey);
        sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
        m_pWebSocketServerSSL->setSslConfiguration(sslConfiguration);

        if (m_pWebSocketServerSSL->listen(QHostAddress::Any, pServerConfig->serverSslPort())) {
            Log::info(TAG, "freehackquest-backend listening (via ssl) on port" + QString::number(pServerConfig->serverSslPort()));
            connect(m_pWebSocketServerSSL, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnectionSSL);
            connect(m_pWebSocketServerSSL, &QWebSocketServer::sslErrors, this, &WebSocketServer::onSslErrors);
        }else{
            Log::err(TAG, "fhq-server can not listening (via ssl) on port " + QString::number(pServerConfig->serverSslPort()));
            m_bFailed = true;
            return;
        }
    }
    connect(this, &WebSocketServer::sig_sendToAll, this, &WebSocketServer::slot_sendToAll);
    connect(this, &WebSocketServer::signal_sendToOne, this, &WebSocketServer::slot_sendToOne);

    // connect(this, SIGNAL(sig_sendToAll(nlohmann::json)), this, SLOT(slot_sendToAll(nlohmann::json)));

    pServerInfo->serverStarted();
    pWsServer->setServer(this); // temporary

    // TODO save in database information about server started
}

// ---------------------------------------------------------------------

WebSocketServer::~WebSocketServer() {
    m_pWebSocketServer->close();
    m_pWebSocketServerSSL->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

// ---------------------------------------------------------------------

bool WebSocketServer::isFailed(){
    return m_bFailed;
}

// ---------------------------------------------------------------------

void WebSocketServer::sendServerMessage(QWebSocket *pSocket){
    nlohmann::json jsonServer;
    jsonServer["cmd"] = "server";
    jsonServer["m"] = "s0";
    jsonServer["app"] = "fhq-server";
    jsonServer["version"] = FHQSRV_VERSION;

    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();
    jsonServer["developers"] = pServerInfo->developers();

    sendMessage(pSocket, jsonServer);
}

// ---------------------------------------------------------------------

/*!
 *  Handling new connection by ws://
 */

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    Log::info(TAG, "NewConnection " + pSocket->peerAddress().toString() + " " + QString::number(pSocket->peerPort()));

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
    sendServerMessage(pSocket);
}

// ---------------------------------------------------------------------

/*!
 *  Handling new connection by wss://
 */

void WebSocketServer::onNewConnectionSSL(){
    QWebSocket *pSocket = m_pWebSocketServerSSL->nextPendingConnection();
    Log::info(TAG, "NewConnectionSSL " + pSocket->peerAddress().toString() + " " + QString::number(pSocket->peerPort()));

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
    sendServerMessage(pSocket);
}

// ---------------------------------------------------------------------

void WebSocketServer::processTextMessage(const QString &message) {
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();
    EmployWsServer *pWsServer = findEmploy<EmployWsServer>();

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    std::string sCmd = "";
    std::string sM = "";
    try{
        if(!nlohmann::json::accept(message.toStdString())){
            this->sendMessageError(pClient, sCmd, sM, Errors::NotImplementedYet("Not JSON data"));
            return;
        }

        nlohmann::json jsonRequest_ = nlohmann::json::parse(message.toStdString());
        ModelRequest *pModelRequest = new ModelRequest(pClient, this, jsonRequest_);
        
        if(!pModelRequest->hasCommand()){
            this->sendMessageError(pClient, sCmd, sM, Errors::NotFound("requare parameter 'cmd'"));
            // pModelRequestData->sendError(Errors::NotFound("command '" + QString(cmd.c_str()) + "'"));
            return;
        }

        sCmd = pModelRequest->command();
        if(!pModelRequest->hasM()){
            Log::info(TAG.toStdString(), "[WS] >>> " + sCmd);
            this->sendMessageError(pClient, sCmd, sM, Errors::NotFound("requare parameter 'm' - messageid"));
            return;
        }
        sM = pModelRequest->m();

        Log::info(TAG.toStdString(), "[WS] >>> " + sCmd + ":" + sM);

        CmdHandlerBase *pCmdHandler = CmdHandlers::findCmdHandler(sCmd);
        if(pCmdHandler == NULL){
            Log::warn(TAG.toStdString(), "Unknown command: " + sCmd);
            pModelRequest->sendMessageError(sCmd, Errors::NotFound("command '" + QString::fromStdString(sCmd) + "'"));
            return;
        }

        pServerInfo->incrementRequests(sCmd);

        // check access
        const ModelCommandAccess access = pCmdHandler->access();
        if(!access.accessUnauthorized()){
            IUserToken *pUserToken = getUserToken(pClient);
            if(pUserToken == NULL){
                pModelRequest->sendMessageError(pCmdHandler->cmd(), Errors::NotAuthorizedRequest());
                return;
            }

            // access user
            if(pUserToken->isUser() && !access.accessUser()){
                pModelRequest->sendMessageError(pCmdHandler->cmd(), Errors::AccessDenyForUser());
                return;
            }

            // access admin
            if(pUserToken->isAdmin() && !access.accessAdmin()){
                pModelRequest->sendMessageError(pCmdHandler->cmd(), Errors::AccessDenyForAdmin());
                return;
            }
        }

        // allow access
        // TODO move to ModelRequest
        Error error = Errors::NoneError();
        if(!pWsServer->validateInputParameters(error, pCmdHandler, jsonRequest_)){
            pModelRequest->sendMessageError(pCmdHandler->cmd(), error);
            return;
        }
        pCmdHandler->handle(pModelRequest);
    } catch (const std::exception &e) {
        this->sendMessageError(pClient, sCmd, sM, Errors::InternalServerError());
        Log::err(TAG, e.what());
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::processBinaryMessage(QByteArray /*message*/) {
    // QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    // NOT SUPPORTED
    // Log::info(TAG, "Binary Message received: " + message.toHex());
    /*if (pClient) {
        pClient->sendBinaryMessage(message);
    }*/
}

// ---------------------------------------------------------------------

void WebSocketServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    Log::info(TAG, "socketDisconnected:" + QString::number((quint64)pClient, 16));
    if (pClient) {
        m_tokens.remove(pClient);
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::onSslErrors(const QList<QSslError> &){
    Log::err(TAG, "Ssl errors occurred");
}

// ---------------------------------------------------------------------

int WebSocketServer::getConnectedUsers(){
    return m_clients.length();
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse){
    if (pClient) {
        std::string cmd = jsonResponse["cmd"];
        std::string m = jsonResponse["m"];
        std::string message = jsonResponse.dump();

        Log::info(TAG.toStdString(), "[WS] <<< " + cmd + ":" + m);
        if(m_clients.contains(pClient)){
            try{
                pClient->sendTextMessage(QString::fromStdString(message));
            }catch(...){
                Log::err(TAG.toStdString(), "Could not send message <<< " + message);
            }
        }else{
            Log::warn(TAG.toStdString(), "Could not send message, client disappeared");
        }
   }else{
        Log::warn(TAG.toStdString(), "Client is wrong");
   }
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessageError(QWebSocket *pClient, const std::string &sCmd, const std::string &sM, Error error){
    nlohmann::json jsonResponse;
    jsonResponse["cmd"] = sCmd;
    jsonResponse["m"] = sM;
    jsonResponse["result"] = "FAIL";
    jsonResponse["error"] = error.message();
    jsonResponse["code"] = error.codeError();
    Log::err(TAG.toStdString(), "WS-ERROR >>> " + sCmd + ":" + sM + ", messsage: " + error.message());
    this->sendMessage(pClient, jsonResponse);
    return;
}

// ---------------------------------------------------------------------

void WebSocketServer::sendToAll(const nlohmann::json& jsonMessage){
    std::string message = jsonMessage.dump();
    emit sig_sendToAll(QString(message.c_str()));
}

// ---------------------------------------------------------------------

void WebSocketServer::sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage){
    std::string message = jsonMessage.dump();
    emit signal_sendToOne(pClient, QString(message.c_str()));
}

// ---------------------------------------------------------------------
// slot
void WebSocketServer::slot_sendToAll(QString message){
    if(!nlohmann::json::accept(message.toStdString())){
        return;
    }
    nlohmann::json jsonMessage = nlohmann::json::parse(message.toStdString());
    for(int i = 0; i < m_clients.size(); i++){
        this->sendMessage(m_clients.at(i), jsonMessage);
    }
}

// ---------------------------------------------------------------------
// slot
void WebSocketServer::slot_sendToOne(QWebSocket *pClient, QString message){
    if(!nlohmann::json::accept(message.toStdString())){
        return;
    }
    nlohmann::json jsonMessage = nlohmann::json::parse(message.toStdString());
    this->sendMessage(pClient, jsonMessage);
}

// ---------------------------------------------------------------------

// TODO move to EmployWsServer

void WebSocketServer::setUserToken(QWebSocket *pClient, IUserToken *pUserToken){
    m_tokens[pClient] = pUserToken;
}

// ---------------------------------------------------------------------

IUserToken * WebSocketServer::getUserToken(QWebSocket *pClient){
    if(m_tokens.contains(pClient)){
        return m_tokens[pClient];
    }
    return NULL;
}

// ---------------------------------------------------------------------
