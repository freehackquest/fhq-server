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
#include <cmd_handlers.h>

#include <error.h>

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
        bool isFailed();

		// IWebSocketServer
		virtual int getConnectedUsers();
        virtual void sendMessage(QWebSocket *pClient, QJsonObject obj); // deprecated
        virtual void sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse);
        virtual void sendMessageError(QWebSocket *pClient, const std::string &cmd, QString m, Error error);
		virtual void sendToAll(QJsonObject obj);
		virtual void setUserToken(QWebSocket *pClient, IUserToken *pUserToken);
		virtual IUserToken * getUserToken(QWebSocket *pClient);

	Q_SIGNALS:
		void closed();

	private Q_SLOTS:
		void onNewConnection();
		void onNewConnectionSSL();
        void processTextMessage(const QString &message);
		void processBinaryMessage(QByteArray message);
		void socketDisconnected();
		void onSslErrors(const QList<QSslError> &errors);

	private:
		QWebSocketServer *m_pWebSocketServer;
		QWebSocketServer *m_pWebSocketServerSSL;
		QList<QWebSocket *> m_clients;
		QMap<QWebSocket *, IUserToken *> m_tokens;

		bool m_bFailed;
		QString TAG;
};

#endif //WEBSOCKETSERVER_H
