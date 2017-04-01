#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMap>
#include <QFile>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>

#include "interfaces/icmdhandler.h"
#include "interfaces/iwebsocketserver.h"
#include "error.h"

// QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
// QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject, public IWebSocketServer {
	
	private:
		Q_OBJECT
	public:
		explicit WebSocketServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
		~WebSocketServer();

		// IWebSocketServer
		virtual int getConnectedUsers();
		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj);
		virtual void sendMessageError(QWebSocket *pClient, QString cmd, Error error);
		virtual void sendToAll(QJsonObject obj);
		virtual QSqlDatabase *database();
		virtual void setUserToken(QWebSocket *pClient, UserToken *pUserToken);
		virtual UserToken * getUserToken(QWebSocket *pClient);
		virtual void sendLettersBcc(QStringList emails, QString subject, QString text);
		
	Q_SIGNALS:
		void closed();

	private Q_SLOTS:
		void onNewConnection();
		void processTextMessage(QString message);
		void processBinaryMessage(QByteArray message);
		void socketDisconnected();

	private:
		QString readStringFromSettings(QSettings &sett, QString settName, QString defaultValue);
		int readIntFromSettings(QSettings &sett, QString settName, int defaultValue);
	
		QWebSocketServer *m_pWebSocketServer;
		QList<QWebSocket *> m_clients;
		QMap<QWebSocket *, UserToken *> m_tokens;
		QMap<QString, ICmdHandler *> m_mapCmdHandlers;
		bool m_debug;
		
		// settings
		QString m_sFilename;
		QString m_sDatabase_host;
		QString m_sDatabase_name;
		QString m_sDatabase_user;
		QString m_sDatabase_password;
		
		QString m_sEmail_smtphost;
		int m_nEmail_smtpport;
		QString m_sEmail_username;
		QString m_sEmail_password;
		
		// db
		QSqlDatabase *m_pDatabase;
		
};

#endif //WEBSOCKETSERVER_H
