#ifndef EMPLOY_WS_SERVER_H
#define EMPLOY_WS_SERVER_H

#include <employees.h>
#include <cmd_handlers.h>
//
#include <QJsonObject>

class EmployWsServer : public EmployBase {
public:
    EmployWsServer();
    static std::string name() { return "EmployWsServer"; }
    virtual bool init();
    bool validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler, const nlohmann::json& jsonMessage);
    void setServer(IWebSocketServer *pWebSocketServer);
    void sendToAll(const nlohmann::json& jsonMessage);
    void sendToOne(QWebSocket *pClient, const nlohmann::json& jsonMessage);

private:
    std::string TAG;
    IWebSocketServer *m_pWebSocketServer;
};

#endif // EMPLOY_WS_SERVER_H
