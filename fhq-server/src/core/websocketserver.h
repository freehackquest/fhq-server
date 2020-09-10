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
#include <mutex>
#include <wsjcpp_jsonrpc20.h>

class WebSocketClient : public QObject, public WsjcppJsonRpc20WebSocketClient {
    private:
        Q_OBJECT
    public:
        WebSocketClient(QWebSocket *pClient);
        QWebSocket *getClient();

        // WsjcppJsonRpc20WebSocketClient
        virtual std::string getPeerIpAddress() override;
        virtual int getPeerPort() override;
        virtual std::string getRequestUrl() override;
        virtual void onDisconnected() override;
        virtual void sendTextMessage(const std::string &sTextMessage) override;
        
    Q_SIGNALS:
        void signal_sendTextMessage(QString sTextMessage);

    private Q_SLOTS:
        void slot_sendTextMessage(QString sTextMessage);

    private:
        std::string TAG;
        QWebSocket *m_pClient;
};

// QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
// QT_FORWARD_DECLARE_CLASS(QWebSocket)

/*! \brief WebSocket Server implementation
 *         Implemented ws://addr:1234 and wss://addr:4613 (optional)
 *
 *  This object handle income connections.
 *  Processing income messages.
 *  Control database connections
 */

class WebSocketServer : public QObject, public WsjcppJsonRpc20WebSocketServer {

    private:
        Q_OBJECT
    public:
        explicit WebSocketServer(QObject *parent = Q_NULLPTR);
        ~WebSocketServer();
        bool isFailed();

        // WsjcppJsonRpc20WebSocketServer
        
        // virtual void setUserSession(void *pClient, WsjcppJsonRpc20UserSession *pWsjcppJsonRpc20UserSession) override; 
        // virtual void unsetUserSession(void *pClient) override;
        // virtual WsjcppJsonRpc20UserSession *findUserSession(void *pClient) override;

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
        void initNewConnection(const std::string &sPrefix, QWebSocket *pSocket);
        void logSocketError(QAbstractSocket::SocketError socketError);

        QWebSocketServer *m_pWebSocketServer;
        QWebSocketServer *m_pWebSocketServerSSL;

        // TODO redesign to std::map and move to EmployWSServer
        // TODO rename m_tokens to m_mapUserSessions;
        // TODO usersession must be single std::map<std::string sUserUuid, WsjcppJsonRpc20UserSession *>
        std::mutex m_mtxUserSession;
        std::map<void *, WsjcppJsonRpc20UserSession *> m_mapUserSession; 
        
        bool m_bFailed;
};

#endif //WEBSOCKETSERVER_H
