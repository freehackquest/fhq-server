#ifndef INTERFACES_IWEBSOCKETSERVER_H
#define INTERFACES_IWEBSOCKETSERVER_H

#include <QJsonDocument> // depricated
#include <QJsonObject>
#include <QWebSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <json.hpp>

#include <iusertoken.h>
#include <error.h>

class IWebSocketServer {
    public:
//        virtual void sendMessage(QWebSocket *pClient, QJsonObject obj) = 0; // deprecated
        virtual void sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse) = 0;
        virtual void sendMessageError(QWebSocket *pClient, const std::string &sCmd, const std::string & sM, Error error) = 0;
//	virtual void sendToAll(QJsonObject obj) = 0;
        virtual void sendToAll(const nlohmann::json& jsonMessage) = 0;
        virtual void sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) = 0;
        virtual int getConnectedUsers() = 0;
        virtual void setUserToken(QWebSocket *pClient, IUserToken *pUserToken) = 0;
        virtual IUserToken * getUserToken(QWebSocket *pClient) = 0;
};

#endif // INTERFACES_IWEBSOCKETSERVER_H
