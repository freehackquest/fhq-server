#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMap>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtNetwork/QSslError>

#include <iserver.h>

#include "error.h"
#include "headers/server_config.h"

// QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
// QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject, public IWebSocketServer {
	
	private:
		Q_OBJECT
	public:
		explicit WebSocketServer(QObject *parent = Q_NULLPTR);
		~WebSocketServer();

		// IWebSocketServer
		virtual int getConnectedUsers();
		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj);
		virtual void sendMessageError(QWebSocket *pClient, QString cmd, Error error);
		virtual void sendToAll(QJsonObject obj);
		virtual QSqlDatabase *database();
		virtual void setUserToken(QWebSocket *pClient, IUserToken *pUserToken);
		virtual IUserToken * getUserToken(QWebSocket *pClient);
		virtual void sendLettersBcc(QStringList emails, QString subject, QString text);
		virtual IMemoryCache *findMemoryCache(QString name);
		
	Q_SIGNALS:
		void closed();

	private Q_SLOTS:
		void onNewConnection();
		void onNewConnectionSSL();
		void processTextMessage(QString message);
		void processBinaryMessage(QByteArray message);
		void socketDisconnected();
		void onSslErrors(const QList<QSslError> &errors);

	private:
		
		QWebSocketServer *m_pWebSocketServer;
		QWebSocketServer *m_pWebSocketServerSSL;
		QList<QWebSocket *> m_clients;
		QMap<QWebSocket *, IUserToken *> m_tokens;
		QMap<QString, ICmdHandler *> m_mapCmdHandlers;
		QMap<QString, IMemoryCache *> m_mapMemoryCache;

		bool m_bFailed;
		ServerConfig* m_pServerConfig;
		
		// db two connections
		QSqlDatabase *m_pDatabase;
		QSqlDatabase *m_pDatabase_older;
};

#endif //WEBSOCKETSERVER_H
