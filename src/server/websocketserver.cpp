#include "websocketserver.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

#include <QHostAddress>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

#include "../cmd/create_cmd_handlers.h"
#include "../smtp/smtp.h"
#include "../updates/create_list_updates.h"

// QT_USE_NAMESPACE

// ---------------------------------------------------------------------

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent) {
	
	// default settings
	m_bFailed = false;
	m_bServer_ssl_on = false;
	m_sDatabase_host = "localhost";
	m_sDatabase_name = "freehackquest";
	m_sDatabase_user = "freehackquest_u";
	m_sDatabase_password = "freehackquest_p";

	m_sEmail_smtphost = "smtp.gmail.com";
	m_nEmail_smtpport = 465;
	m_sEmail_username = "";
	m_sEmail_password = "";

	m_nServer_port = 1234;
	m_bServer_ssl_on = false;
	m_nServer_ssl_port = 4613;
	m_sServer_ssl_key_file = "/etc/ssl/private/localhost.key";
	m_sServer_ssl_cert_file = "/etc/ssl/certs/localhost.pem";

	m_pWebSocketServer = new QWebSocketServer(QStringLiteral("freehackquest-backend"), QWebSocketServer::NonSecureMode, this);
	m_pWebSocketServerSSL = new QWebSocketServer(QStringLiteral("freehackquest-backend"), QWebSocketServer::SecureMode, this);
	m_sFilename = "/etc/freehackquest-backend/conf.ini";
	if(QFile::exists(m_sFilename)){
		QSettings sett(m_sFilename, QSettings::IniFormat);
		m_sDatabase_host = readStringFromSettings(sett, "DATABASE/host", m_sDatabase_host);
		m_sDatabase_name = readStringFromSettings(sett, "DATABASE/name", m_sDatabase_name);
		m_sDatabase_user = readStringFromSettings(sett, "DATABASE/user", m_sDatabase_user);
		m_sDatabase_password = readStringFromSettings(sett, "DATABASE/password", m_sDatabase_password);
		
		qDebug() << "Database_host: " << m_sDatabase_host;
		qDebug() << "Database_name: " << m_sDatabase_name;
		qDebug() << "Database_user: " << m_sDatabase_user;
		// qDebug() << "Database_password: " << m_sDatabase_password;

		m_pDatabase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
		m_pDatabase->setHostName(m_sDatabase_host);
		m_pDatabase->setDatabaseName(m_sDatabase_name);
		m_pDatabase->setUserName(m_sDatabase_user);
		m_pDatabase->setPassword(m_sDatabase_password);
		if (!m_pDatabase->open()){
			qDebug() << m_pDatabase->lastError().text();
			qDebug() << "Failed to connect.";
			m_bFailed = true;
			return;
		}else{
			qDebug() << "Success connection to database";
			tryUpdateDatabase(m_pDatabase);
		}
		
		m_sEmail_smtphost = readStringFromSettings(sett, "EMAIL/host", m_sEmail_smtphost);
		m_nEmail_smtpport = readIntFromSettings(sett, "EMAIL/port", m_nEmail_smtpport);
		m_sEmail_username = readStringFromSettings(sett, "EMAIL/username", m_sEmail_username);
		m_sEmail_password = readStringFromSettings(sett, "EMAIL/password", m_sEmail_password);

		m_nServer_port = readIntFromSettings(sett, "SERVER/port", m_nServer_port);
		m_bServer_ssl_on = readBoolFromSettings(sett, "SERVER/ssl_on", m_bServer_ssl_on);
		m_nServer_ssl_port = readIntFromSettings(sett, "SERVER/ssl_port", m_nServer_ssl_port);
		m_sServer_ssl_key_file = readStringFromSettings(sett, "SERVER/ssl_key_file", m_sServer_ssl_key_file);
		m_sServer_ssl_cert_file = readStringFromSettings(sett, "SERVER/ssl_cert_file", m_sServer_ssl_cert_file);
	}else{
		qDebug() << "Not found config file " << m_sFilename;
		m_bFailed = true;
		return;
	}

    if (m_pWebSocketServer->listen(QHostAddress::Any, m_nServer_port)) {
		qDebug() << "freehackquest-backend listening on port" << m_nServer_port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
        create_cmd_handlers(m_mapCmdHandlers);
    }else{
		qDebug() << "ERROR: freehackquest-backend can not listening on port " << m_nServer_port;
		m_bFailed = true;
		return;
	}

	if(m_bServer_ssl_on){
		QSslConfiguration sslConfiguration;
		QFile certFile(m_sServer_ssl_cert_file);
		QFile keyFile(m_sServer_ssl_key_file);
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
		
		if (m_pWebSocketServerSSL->listen(QHostAddress::Any, m_nServer_ssl_port)) {
			qDebug() << "freehackquest-backend listening (via ssl) on port" << m_nServer_ssl_port;
			connect(m_pWebSocketServerSSL, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnectionSSL);
			connect(m_pWebSocketServerSSL, &QWebSocketServer::sslErrors, this, &WebSocketServer::onSslErrors);
		}else{
			qDebug() << "ERROR: freehackquest-backend can not listening (via ssl) on port" << m_nServer_ssl_port;
			m_bFailed = true;
			return;
		}
	}
}

// ---------------------------------------------------------------------

WebSocketServer::~WebSocketServer() {
    m_pWebSocketServer->close();
    m_pWebSocketServerSSL->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

// ---------------------------------------------------------------------

QString WebSocketServer::readStringFromSettings(QSettings &sett, QString settName, QString defaultValue){
	QString sResult = defaultValue;
	if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toString();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return sResult;
}

// ---------------------------------------------------------------------

int WebSocketServer::readIntFromSettings(QSettings &sett, QString settName, int defaultValue){
	int nResult = defaultValue;
	if(sett.contains(settName)){
		nResult = sett.value(settName, nResult).toInt();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return nResult;
}

// ---------------------------------------------------------------------

bool WebSocketServer::readBoolFromSettings(QSettings &sett, QString settName, bool defaultValue){
	bool bResult = defaultValue;
	if(sett.contains(settName)){
		bResult = sett.value(settName, bResult).toBool();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return bResult;
}

// ---------------------------------------------------------------------

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
	qDebug() << "NewConnection " << pSocket->peerAddress().toString() << " " << pSocket->peerPort();
        
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
}

// ---------------------------------------------------------------------

void WebSocketServer::onNewConnectionSSL(){
	QWebSocket *pSocket = m_pWebSocketServerSSL->nextPendingConnection();
	qDebug() << "NewConnectionSSL " << pSocket->peerAddress().toString() << " " << pSocket->peerPort();
        
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
}

// ---------------------------------------------------------------------

void WebSocketServer::processTextMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] <<< " << message;

	QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jsonData = doc.object();
	
	if(jsonData.contains("cmd")){
		QString cmd = jsonData["cmd"].toString();
		
		if(m_mapCmdHandlers.contains(cmd)){
			ICmdHandler *pCmdHandler = m_mapCmdHandlers[cmd];
			
			// check access
			if(!pCmdHandler->accessUnauthorized()){
				UserToken *pUserToken = getUserToken(pClient);
				if(pUserToken == NULL){
					QJsonObject jsonData;
					jsonData["cmd"] = QJsonValue(pCmdHandler->cmd());
					jsonData["result"] = QJsonValue("FAIL");
					jsonData["error"] = QJsonValue("Not authorized request");
					sendMessage(pClient, jsonData);
					return;
				}
				
				// access user
				if(pUserToken->isUser() && !pCmdHandler->accessUser()){
					QJsonObject jsonData;
					jsonData["cmd"] = QJsonValue(pCmdHandler->cmd());
					jsonData["result"] = QJsonValue("FAIL");
					jsonData["error"] = QJsonValue("Access deny for user");
					sendMessage(pClient, jsonData);
					return;
				}
				
				// access tester
				if(pUserToken->isTester() && !pCmdHandler->accessTester()){
					QJsonObject jsonData;
					jsonData["cmd"] = QJsonValue(pCmdHandler->cmd());
					jsonData["result"] = QJsonValue("FAIL");
					jsonData["error"] = QJsonValue("Access deny for tester");
					sendMessage(pClient, jsonData);
					return;
				}
				
				// access admin
				if(pUserToken->isAdmin() && !pCmdHandler->accessAdmin()){
					QJsonObject jsonData;
					jsonData["cmd"] = QJsonValue(pCmdHandler->cmd());
					jsonData["result"] = QJsonValue("FAIL");
					jsonData["error"] = QJsonValue("Access deny for admin");
					sendMessage(pClient, jsonData);
					return;
				}

				// allow
				pCmdHandler->handle(pClient, this, jsonData);	
				
			}else{
				// allow unauthorized request
				pCmdHandler->handle(pClient, this, jsonData);	
			}
		}else{
			qDebug() << "Unknown command: " << cmd;
			QJsonObject jsonData;
			jsonData["cmd"] = QJsonValue(cmd);
			jsonData["error"] = QString("Unknown command");
			this->sendMessage(pClient, jsonData);
		}
	}else{
		QJsonObject jsonData;
		jsonData["error"] = QString("Invalid command format");
		this->sendMessage(pClient, jsonData);
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    
	qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
		m_tokens.remove(pClient);
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::onSslErrors(const QList<QSslError> &){
    qDebug() << "Ssl errors occurred";
}

// ---------------------------------------------------------------------

int WebSocketServer::getConnectedUsers(){
	return m_clients.length();
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessage(QWebSocket *pClient, QJsonObject obj){
	 if (pClient) {
		QJsonDocument doc(obj);
		QString message = doc.toJson(QJsonDocument::Compact);
		qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] >>> " << message;
        pClient->sendTextMessage(message);
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessageError(QWebSocket *pClient, QString cmd, Error error){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd);
	jsonData["result"] = QJsonValue("FAIL");
	jsonData["error"] = QJsonValue(error.message());
	jsonData["code"] = QJsonValue(error.codeError());
	this->sendMessage(pClient, jsonData);
	return;
}

// ---------------------------------------------------------------------

void WebSocketServer::sendToAll(QJsonObject obj){
	for(int i = 0; i < m_clients.size(); i++){
		this->sendMessage(m_clients.at(i), obj);
	}
}

// ---------------------------------------------------------------------

QSqlDatabase *WebSocketServer::database(){
	
	if(!m_pDatabase->isOpen()){
		qDebug() << "Database is not open";
	}
	
	if(m_pDatabase->isOpenError()){
		qDebug() << "Database connection has error";
	}
	
	if(!m_pDatabase->isValid()){
		qDebug() << "Database connection invalid";
	}

	qDebug() << "Database last error: " << m_pDatabase->lastError();

	return m_pDatabase;
}

// ---------------------------------------------------------------------

void WebSocketServer::setUserToken(QWebSocket *pClient, UserToken *pUserToken){
	m_tokens[pClient] = pUserToken;
}

// ---------------------------------------------------------------------

UserToken * WebSocketServer::getUserToken(QWebSocket *pClient){
	if(m_tokens.contains(pClient)){
		return m_tokens[pClient];
	}
	return NULL;
}

// ---------------------------------------------------------------------

void WebSocketServer::sendLettersBcc(QStringList emails, QString subject, QString text){
	Smtp* smtp = new Smtp(m_sEmail_username, m_sEmail_password, m_sEmail_smtphost, m_nEmail_smtpport);
	// smtp->disableDebugMode();
    //connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
	smtp->sendMailBcc("freehackquest@gmail.com", emails, subject, text);
}

// ---------------------------------------------------------------------
