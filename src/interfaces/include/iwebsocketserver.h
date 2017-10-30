#ifndef INTERFACES_IWEBSOCKETSERVER_H
#define INTERFACES_IWEBSOCKETSERVER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>

#include <icmdhandler.h>
#include <imemorycache.h>
#include <iusertoken.h>
#include <error.h>
#include <errors.h>

class IWebSocketServer {
	public:
		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj) = 0;
		virtual void sendMessageError(QWebSocket *pClient, QString cmd, QString m, Error error) = 0;
		virtual void sendToAll(QJsonObject obj) = 0;
		virtual int getConnectedUsers() = 0;
		virtual QSqlDatabase *database() = 0;
		virtual void setUserToken(QWebSocket *pClient, IUserToken *pUserToken) = 0;
		virtual IUserToken * getUserToken(QWebSocket *pClient) = 0;
		virtual IMemoryCache *findMemoryCache(QString name) = 0;
		virtual void exportApi(QJsonObject &result) = 0;
};

#endif // INTERFACES_IWEBSOCKETSERVER_H
