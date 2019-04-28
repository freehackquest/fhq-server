#include <websocketserver.h>

#include <QtCore> // TODO remove
#include <QDateTime>
#include <QRegularExpression>

#include <QHostAddress>
#include <QThread>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

#include <utils_logger.h>

#include <employ_server_config.h>
#include <employ_server_info.h>
#include <employ_ws_server.h>
#include <cmd_handlers.h>

// QT_USE_NAMESPACE

// ---------------------------------------------------------------------

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent) {
    TAG = "WebSocketServer";

    m_bFailed = false;
    if (!Employees::init({"start_ws_server"})) {
        m_bFailed = true;
        return;
    }

    EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
    EmployServerInfo *pServerInfo = findEmploy<EmployServerInfo>();
    EmployWsServer *pWsServer = findEmploy<EmployWsServer>();

    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("fhq-server"), QWebSocketServer::NonSecureMode, this);
    m_pWebSocketServerSSL = new QWebSocketServer(QStringLiteral("fhq-server"), QWebSocketServer::SecureMode, this);

    if (m_pWebSocketServer->listen(QHostAddress::Any, pServerConfig->serverPort())) {
        Log::info(TAG, "fhq-server listening on port " + std::to_string(pServerConfig->serverPort()));
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
    } else {
        Log::err(TAG, "fhq-server can not listening on port " + std::to_string(pServerConfig->serverPort()));
        m_bFailed = true;
        return;
    }

    if (pServerConfig->serverSslOn()) {
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
            Log::info(TAG, "fhq-server listening (via ssl) on port" + std::to_string(pServerConfig->serverSslPort()));
            connect(m_pWebSocketServerSSL, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnectionSSL);
            connect(m_pWebSocketServerSSL, &QWebSocketServer::sslErrors, this, &WebSocketServer::onSslErrors);
        } else {
            Log::err(TAG, "fhq-server can not listening (via ssl) on port " + std::to_string(pServerConfig->serverSslPort()));
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

bool WebSocketServer::isFailed() {
    return m_bFailed;
}

// ---------------------------------------------------------------------

void WebSocketServer::sendServerMessage(QWebSocket *pSocket) {
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
    std::string sAddress = pSocket->peerAddress().toString().toStdString();
    Log::info(TAG, "NewConnection " + sAddress + " " + std::to_string(pSocket->peerPort()));

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

void WebSocketServer::onNewConnectionSSL() {
    QWebSocket *pSocket = m_pWebSocketServerSSL->nextPendingConnection();
    std::string sAddress = pSocket->peerAddress().toString().toStdString();
    Log::info(TAG, "NewConnectionSSL " + sAddress + " " + std::to_string(pSocket->peerPort()));

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
    try {
        if (!nlohmann::json::accept(message.toStdString())) {
            this->sendMessageError(pClient, sCmd, sM, Error(400, "Not JSON data"));
            return;
        }

        nlohmann::json jsonRequest_ = nlohmann::json::parse(message.toStdString());
        ModelRequest *pModelRequest = new ModelRequest(pClient, this, jsonRequest_);
        
        if (!pModelRequest->hasCommand()) {
            this->sendMessageError(pClient, sCmd, sM, Error(404, "Not found requare parameter 'cmd'"));
            // pModelRequestData->sendError(Error(404, "Not found command '" + QString(cmd.c_str()) + "'"));
            return;
        }

        sCmd = pModelRequest->command();
        if (!pModelRequest->hasM()) {
            Log::info(TAG, "[WS] >>> " + sCmd);
            this->sendMessageError(pClient, sCmd, sM, Error(404, "Not found requare parameter 'm' - messageid"));
            return;
        }
        sM = pModelRequest->m();

        Log::info(TAG, "[WS] >>> " + sCmd + ":" + sM);

        CmdHandlerBase *pCmdHandler = CmdHandlers::findCmdHandler(sCmd);
        if (pCmdHandler == NULL) {
            Log::warn(TAG, "Unknown command: " + sCmd);
            pModelRequest->sendMessageError(sCmd, Error(404, "Not found command '" + sCmd + "'"));
            return;
        }

        pServerInfo->incrementRequests(sCmd);

        // check access
        // TODO move check inside in CmdHandler
        if (!pCmdHandler->checkAccess(pModelRequest)) {
            return;
        }

        // allow access
        // TODO move to ModelRequest
        Error error(404, "none");
        if (!pWsServer->validateInputParameters(error, pCmdHandler, jsonRequest_)) {
            pModelRequest->sendMessageError(pCmdHandler->cmd(), error);
            return;
        }
        pCmdHandler->handle(pModelRequest);
    } catch (const std::exception &e) {
        this->sendMessageError(pClient, sCmd, sM, Error(500, "InternalServerError"));
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
    long long nClient = (long long) pClient;
    // TODO hex print
    Log::info(TAG, "socketDisconnected:" + std::to_string(nClient));
    if (pClient) {
        m_tokens.remove(pClient);
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::onSslErrors(const QList<QSslError> &) {
    Log::err(TAG, "Ssl errors occurred");
}

// ---------------------------------------------------------------------

int WebSocketServer::getConnectedUsers() {
    return m_clients.length();
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse) {
    if (pClient) {
        std::string sCmd = "";
        if (jsonResponse.find("cmd") != jsonResponse.end() && jsonResponse["cmd"].is_string()) {
            sCmd = jsonResponse["cmd"];
        }
        std::string sM = "";
        
        if (jsonResponse.find("m") != jsonResponse.end() && jsonResponse["m"].is_string()) {
            sM = jsonResponse["m"];
        }
        std::string sMessage = jsonResponse.dump();

        Log::info(TAG, "[WS] <<< " + sCmd + ":" + sM);
        if (m_clients.contains(pClient)) {
            try {
                pClient->sendTextMessage(QString::fromStdString(sMessage));
            } catch(...) {
                Log::err(TAG, "Could not send message <<< " + sMessage);
            }
        } else {
            Log::warn(TAG, "Could not send message, client disappeared");
        }
   } else {
        Log::warn(TAG, "Client is wrong");
   }
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessageError(QWebSocket *pClient, const std::string &sCmd, const std::string &sM, Error error) {
    nlohmann::json jsonResponse;
    jsonResponse["cmd"] = sCmd;
    jsonResponse["m"] = sM;
    jsonResponse["result"] = "FAIL";
    jsonResponse["error"] = error.message();
    jsonResponse["code"] = error.codeError();
    Log::err(TAG, "WS-ERROR >>> " + sCmd + ":" + sM + ", messsage: " + error.message());
    this->sendMessage(pClient, jsonResponse);
    return;
}

// ---------------------------------------------------------------------

void WebSocketServer::sendToAll(const nlohmann::json& jsonMessage) {
    std::string message = jsonMessage.dump();
    emit sig_sendToAll(QString(message.c_str()));
}

// ---------------------------------------------------------------------

void WebSocketServer::sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) {
    std::string message = jsonMessage.dump();
    emit signal_sendToOne(pClient, QString(message.c_str()));
}

// ---------------------------------------------------------------------
// slot
void WebSocketServer::slot_sendToAll(QString message) {
    std::string sMsg = message.toStdString();
    if (!nlohmann::json::accept(sMsg)) {
        return;
    }
    nlohmann::json jsonMessage = nlohmann::json::parse(sMsg);
    for (int i = 0; i < m_clients.size(); i++) {
        this->sendMessage(m_clients.at(i), jsonMessage);
    }
}

// ---------------------------------------------------------------------
// slot
void WebSocketServer::slot_sendToOne(QWebSocket *pClient, QString message) {
    if (!nlohmann::json::accept(message.toStdString())) {
        return;
    }
    nlohmann::json jsonMessage = nlohmann::json::parse(message.toStdString());
    this->sendMessage(pClient, jsonMessage);
}

// ---------------------------------------------------------------------

// TODO move to EmployWsServer

void WebSocketServer::setWSJCppUserSession(QWebSocket *pClient, WSJCppUserSession *pWSJCppUserSession) {
    m_tokens[pClient] = pWSJCppUserSession;
}

// ---------------------------------------------------------------------
// TODO EmployWsServer

WSJCppUserSession *WebSocketServer::getWSJCppUserSession(QWebSocket *pClient) {
    if (m_tokens.contains(pClient)) {
        return m_tokens[pClient];
    }
    return nullptr;
};

// ---------------------------------------------------------------------

void WebSocketServer::logSocketError(QAbstractSocket::SocketError socketError) {
    std::string msg = "Unknown error";
    if (socketError == QAbstractSocket::ConnectionRefusedError) {
        msg = "QAbstractSocket::ConnectionRefusedError, The connection was refused by the peer (or timed out).";
    } else if (socketError == QAbstractSocket::RemoteHostClosedError) {
        msg = "QAbstractSocket::RemoteHostClosedError, The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.";
    } else if (socketError == QAbstractSocket::HostNotFoundError) {
        msg = "QAbstractSocket::HostNotFoundError, The host address was not found.";
    } else if (socketError == QAbstractSocket::SocketAccessError) {
        msg = "QAbstractSocket::SocketAccessError, The socket operation failed because the application lacked the required privileges.";
    } else if (socketError == QAbstractSocket::SocketResourceError) {
        msg = "QAbstractSocket::SocketResourceError, The local system ran out of resources (e.g., too many sockets).";
    } else if (socketError == QAbstractSocket::SocketTimeoutError) {
        msg = "QAbstractSocket::SocketTimeoutError, The socket operation timed out.";
    } else if (socketError == QAbstractSocket::DatagramTooLargeError) {
        msg = "QAbstractSocket::DatagramTooLargeError, The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).";
    } else if (socketError == QAbstractSocket::NetworkError) {
        msg = "QAbstractSocket::NetworkError, An error occurred with the network (e.g., the network cable was accidentally plugged out).";
    } else if (socketError == QAbstractSocket::AddressInUseError) {
        msg = "QAbstractSocket::AddressInUseError, The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.";
    } else if (socketError == QAbstractSocket::SocketAddressNotAvailableError) {
        msg = "QAbstractSocket::SocketAddressNotAvailableError, The address specified to QAbstractSocket::bind() does not belong to the host.";
    } else if (socketError == QAbstractSocket::UnsupportedSocketOperationError) {
        msg = "QAbstractSocket::UnsupportedSocketOperationError, The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).";
    } else if (socketError == QAbstractSocket::ProxyAuthenticationRequiredError) {
        msg = "QAbstractSocket::ProxyAuthenticationRequiredError, The socket is using a proxy, and the proxy requires authentication.";
    } else if (socketError == QAbstractSocket::SslHandshakeFailedError) {
        msg = "QAbstractSocket::SslHandshakeFailedError, The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)";
    } else if (socketError == QAbstractSocket::UnfinishedSocketOperationError) {
        msg = "QAbstractSocket::UnfinishedSocketOperationError, Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).";
    } else if (socketError == QAbstractSocket::ProxyConnectionRefusedError) {
        msg = "QAbstractSocket::ProxyConnectionRefusedError, Could not contact the proxy server because the connection to that server was denied";
    } else if (socketError == QAbstractSocket::ProxyConnectionClosedError) {
        msg = "QAbstractSocket::ProxyConnectionClosedError, The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)";
    } else if (socketError == QAbstractSocket::ProxyConnectionTimeoutError) {
        msg = "QAbstractSocket::ProxyConnectionTimeoutError, The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.";
    } else if (socketError == QAbstractSocket::ProxyNotFoundError) {
        msg = "QAbstractSocket::ProxyNotFoundError, The proxy address set with setProxy() (or the application proxy) was not found.";
    } else if (socketError == QAbstractSocket::ProxyProtocolError) {
        msg = "QAbstractSocket::ProxyProtocolError, The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.";
    } else if (socketError == QAbstractSocket::OperationError) {
        msg = "QAbstractSocket::OperationError, An operation was attempted while the socket was in a state that did not permit it.";
    } else if (socketError == QAbstractSocket::SslInternalError) {
        msg = "QAbstractSocket::SslInternalError, The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library.";
    } else if (socketError == QAbstractSocket::SslInvalidUserDataError) {
        msg = "QAbstractSocket::SslInvalidUserDataError, Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.";
    } else if (socketError == QAbstractSocket::TemporaryError) {
        msg = "QAbstractSocket::TemporaryError, A temporary error occurred (e.g., operation would block and socket is non-blocking).";
    } else if (socketError == QAbstractSocket::UnknownSocketError) {
        msg = "QAbstractSocket::UnknownSocketError, An unidentified error occurred.";
    }
    Log::err(TAG, msg);
}
