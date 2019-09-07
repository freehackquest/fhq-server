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

#include <cmd_handlers.h>

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
        virtual int getConnectedUsers() override;
        virtual void sendMessage(QWebSocket *pClient, const nlohmann::json& jsonResponse) override;
        virtual void sendMessageError(QWebSocket *pClient, const std::string &sCmd, const std::string & sM, WSJCppError error) override;
        virtual void sendToAll(const nlohmann::json& jsonMessage) override;
        void sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) override;
        virtual void setWSJCppUserSession(QWebSocket *pClient, WSJCppUserSession *pWSJCppUserSession) override; 
        virtual WSJCppUserSession *getWSJCppUserSession(QWebSocket *pClient) override;
        void removeWSJCppUserSession(QWebSocket *pClient);

    Q_SIGNALS:
        void closed();
        void sig_sendToAll(QString jsonMessage);
        void signal_sendToOne(QWebSocket *pClient, QString jsonMessage);

    private Q_SLOTS:
        void onNewConnection();
        void onNewConnectionSSL();
        void processTextMessage(const QString &message);
        void processBinaryMessage(QByteArray message);
        void socketDisconnected();
        void onSslErrors(const QList<QSslError> &errors);
        void slot_sendToAll(QString jsonMessage);
        void slot_sendToOne(QWebSocket *pClient, QString message);

    private:
        void initNewConnection(const std::string &sPrefix, QWebSocket *pSocket);
        void sendServerMessage(QWebSocket *pSocket);
        void logSocketError(QAbstractSocket::SocketError socketError);

        QWebSocketServer *m_pWebSocketServer;
        QWebSocketServer *m_pWebSocketServerSSL;
        QList<QWebSocket *> m_clients;
        std::vector<WSJCppSocketClient *> m_vClients;
        // TODO redesign to std::map and move to EmployWSServer
        // TODO rename m_tokens to m_mapUserSessions;
        // TODO usersession must be single std::map<std::string sUserUuid, WSJCppUserSession *>
        std::mutex m_mtxUserSession;
        std::map<QWebSocket *, WSJCppUserSession *> m_mapUserSession; 

        bool m_bFailed;
        std::string TAG;
};

#endif //WEBSOCKETSERVER_H
