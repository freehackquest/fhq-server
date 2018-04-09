#include <websocketserver.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QRegularExpression>

#include <QHostAddress>
#include <QThread>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

#include <SmtpMime>
#include <create_memory_cache.h>
#include <log.h>

#include <employ_server_config.h>
#include <employ_database.h>
#include <employ_settings.h>
#include <model_request.h>
#include <cmd_handlers.h>

// QT_USE_NAMESPACE

// ---------------------------------------------------------------------

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent) {
	TAG = "WebSocketServer";

	m_bFailed = false;
	if(!Employees::init({})){
		m_bFailed = true;
        return;
	}
	
	EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
    
	create_memory_cache(m_mapMemoryCache, this);

	{
		// init memory cache server info
		m_pMemoryCacheServerInfo = NULL;
		IMemoryCache *pMemoryCache = findMemoryCache("serverinfo");
		if(pMemoryCache != NULL){
			m_pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);
            m_pMemoryCacheServerInfo->initCounters();
		}
	}
	
	m_pWebSocketServer = new QWebSocketServer(QStringLiteral("freehackquest-backend"), QWebSocketServer::NonSecureMode, this);
	m_pWebSocketServerSSL = new QWebSocketServer(QStringLiteral("freehackquest-backend"), QWebSocketServer::SecureMode, this);
	
    if (m_pWebSocketServer->listen(QHostAddress::Any, pServerConfig->serverPort())) {
		Log::info(TAG, "freehackquest-backend listening on port" + QString::number(pServerConfig->serverPort()));
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
    }else{
		Log::err(TAG, "freehackquest-backend can not listening on port " + QString::number(pServerConfig->serverPort()));
		m_bFailed = true;
		return;
	}

	if(pServerConfig->serverSslOn()){
		QSslConfiguration sslConfiguration;
		QFile certFile(pServerConfig->serverSslCertFile());
		QFile keyFile(pServerConfig->serverSslKeyFile());
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
			Log::err(TAG, "freehackquest-backend can not listening (via ssl) on port" + QString::number(pServerConfig->serverSslPort()));
			m_bFailed = true;
			return;
		}
	}
    m_pMemoryCacheServerInfo->serverStarted();
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
}

// ---------------------------------------------------------------------

void WebSocketServer::processTextMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    Log::info(TAG, "[WS] <<< " + message);

    nlohmann::json jsonRequest_ = nlohmann::json::parse(message.toStdString());

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8()); // TODO deprecated
    QJsonObject jsonRequest = doc.object(); // TODO deprecated

    ModelRequest *pModelRequest = new ModelRequest(pClient, this, jsonRequest, jsonRequest_);

    if(!pModelRequest->hasCommand()){
		this->sendMessageError(pClient, "error", "", Errors::NotFound("requare parameter 'cmd'"));
		// pModelRequestData->sendError(Errors::NotFound("command '" + QString(cmd.c_str()) + "'"));
		return;
	}
	
    std::string cmd = pModelRequest->command();
	
    if(!pModelRequest->hasM()){
		this->sendMessageError(pClient, cmd, "", Errors::NotFound("requare parameter 'm' - messageid"));
		return;
	}

    ICmdHandler *pCmdHandler = findCmdHandler(cmd);
    if(pCmdHandler == NULL){
        Log::warn(TAG, "Unknown command: " + QString(cmd.c_str()));
        pModelRequest->sendMessageError(cmd, Errors::NotFound("command '" + QString(cmd.c_str()) + "'"));
        return;
    }
	
	if(m_pMemoryCacheServerInfo != NULL){
        m_pMemoryCacheServerInfo->incrementRequests(QString(cmd.c_str()));
	}
	
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
	Error error = Errors::NoneError();
    if(this->validateInputParameters(error, pCmdHandler, jsonRequest)){
        pCmdHandler->handle(pModelRequest);
	}else{
        pModelRequest->sendMessageError(pCmdHandler->cmd(), error);
		return;
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

// deprecated
void WebSocketServer::sendMessage(QWebSocket *pClient, QJsonObject obj){
	 if (pClient) {
		QJsonDocument doc(obj);
		QString message = doc.toJson(QJsonDocument::Compact);
		// Log::info(TAG, QDateTime::currentDateTimeUtc().toString() + " [WS] >>> " + message);
        if(m_clients.contains(pClient)){
            try{
                pClient->sendTextMessage(message);
            }catch(...){
                Log::err(TAG, "Could not send message >>> " + message);
            }
        }
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse){
    if (pClient) {
       std::string message = jsonResponse.dump();
       if(m_clients.contains(pClient)){
           try{
               pClient->sendTextMessage(QString(message.c_str()));
           }catch(...){
               Log::err(TAG.toStdString(), "Could not send message >>> " + message);
           }
       }
   }
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessageError(QWebSocket *pClient, const std::string &cmd, QString m, Error error){
    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(QString(cmd.c_str()));
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["result"] = QJsonValue("FAIL");
    jsonResponse["error"] = QJsonValue(error.message());
    jsonResponse["code"] = QJsonValue(error.codeError());
    this->sendMessage(pClient, jsonResponse);
	return;
}

// ---------------------------------------------------------------------

void WebSocketServer::sendToAll(QJsonObject obj){
	for(int i = 0; i < m_clients.size(); i++){
		this->sendMessage(m_clients.at(i), obj);
	}
}

// ---------------------------------------------------------------------

// TODO move to EmployUsers

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

IMemoryCache *WebSocketServer::findMemoryCache(QString name){
	if(m_mapMemoryCache.contains(name)){
		return m_mapMemoryCache[name];
	}
	return NULL;
}

// ---------------------------------------------------------------------

// TODO move to EmployValidateInput
bool WebSocketServer::validateInputParameters(Error &error, ICmdHandler *pCmdHandler, QJsonObject &jsonRequest){
    const std::vector<CmdInputDef> vInputs = pCmdHandler->inputs();
    for(unsigned int i = 0; i < vInputs.size(); i++){
		CmdInputDef inDef = vInputs[i];
        QString sParamName = QString(inDef.getName().c_str());
        if(inDef.isRequired() && !jsonRequest.contains(sParamName)){
			error = Errors::ParamExpected(sParamName);
			return false;
		}
		
        if(jsonRequest.contains(sParamName)){
			if(inDef.isInteger()){
                QJsonValueRef vParam = jsonRequest[sParamName];
				if(!vParam.isDouble()){
					error = Errors::ParamMustBeInteger(sParamName);
					return false;
				}
                int val = jsonRequest[sParamName].toInt();
				if(inDef.isMinVal() && val < inDef.getMinVal()){
					error = Errors::ParamMustBeMoreThen(sParamName, inDef.getMinVal());
					return false;
				}
				if(inDef.isMaxVal() && val > inDef.getMaxVal()){
					error = Errors::ParamMustBeLessThen(sParamName, inDef.getMaxVal());
					return false;
				}
			}
			
			if(inDef.isEnum()){
                QString val = jsonRequest[sParamName].toString().trimmed();
				QStringList eList = inDef.getEnumList();
				if(!eList.contains(val)){
					error = Errors::ParamExpectedValueOneFrom(sParamName,eList);
					return false;
				}
			}
			
			if(inDef.isUUID()){
                QString val = jsonRequest[sParamName].toString();
                QRegularExpression rx("[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}");
                if(!rx.isValid()){
					Log::err(TAG, "validateInputParameters, " + rx.errorString());
				}
                if(!rx.match(val).hasMatch()){
					error = Errors::ParamExpectedUUID(sParamName);
					return false;
				}
			}

            if(inDef.isEmail()){
                QString val = jsonRequest[sParamName].toString();
                QRegularExpression rx("^[0-9a-zA-Z]{1}[0-9a-zA-Z-._]*[0-9a-zA-Z]{1}@[0-9a-zA-Z]{1}[-.0-9a-zA-Z]*[0-9a-zA-Z]{1}\\.[a-zA-Z]{2,6}$");
                if(!rx.isValid()){
                    Log::err(TAG, "validateInputParameters, " + rx.errorString());
                }
                if(!rx.match(val).hasMatch()){
                    error = Errors::ParamMustBeEmail(sParamName);
                    return false;
                }
            }
		}
	}
	return true;
}

// ---------------------------------------------------------------------
