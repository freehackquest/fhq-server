/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#pragma once

#include <QFile>
#include <QMap>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QtNetwork/QSslError>
#include <mutex>

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
  virtual void sendMessage(QWebSocket *pClient, const nlohmann::json &jsonResponse) override;
  virtual void sendMessageError(QWebSocket *pClient, const std::string &sCmd, const std::string &sM,
                                WsjcppJsonRpc20Error error) override;
  virtual void sendToAll(const nlohmann::json &jsonMessage) override;
  void sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) override;
  virtual void setWsjcppUserSession(QWebSocket *pClient, WsjcppUserSession *pWsjcppUserSession) override;
  virtual WsjcppUserSession *getWsjcppUserSession(QWebSocket *pClient) override;
  void removeWsjcppUserSession(QWebSocket *pClient);

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
  std::vector<WsjcppSocketClient *> m_vClients;
  // TODO redesign to std::map and move to EmployWSServer
  // TODO rename m_tokens to m_mapUserSessions;
  // TODO usersession must be single std::map<std::string sUserUuid,
  // WsjcppUserSession *>
  std::mutex m_mtxUserSession;
  std::map<QWebSocket *, WsjcppUserSession *> m_mapUserSession;

  bool m_bFailed;
  std::string TAG;
};
