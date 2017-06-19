#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMap>
#include <QFile>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtNetwork/QSslError>

#include <iserver.h>

#include <error.h>
#include <server_config.h>
#include <database_connection.h>
#include <memory_cache_serverinfo.h>

// QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
// QT_FORWARD_DECLARE_CLASS(QWebSocket)

/*! \brief WebSocket Server implementation
 *         Implemented ws://addr:1234 and wss://addr:4613 (optional)
 *
 *  This object handle income connections.
 *  Processing income messages.
 *  Control database connections
 */
 
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
		virtual void exportApi(QJsonObject &result);
		
		
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
		MemoryCacheServerInfo *m_pMemoryCacheServerInfo;

		bool m_bFailed;
		ServerConfig* m_pServerConfig;
		
		// db two connections
		QMutex m_mtxSwapConenctions;
		DatabaseConnection *m_pDBConnection;
		DatabaseConnection *m_pDBConnection_older;
};

#endif //WEBSOCKETSERVER_H
