#ifndef CMD_GAME_EXPORT_HANDLER_H
#define CMD_GAME_EXPORT_HANDLER_H

#include <iserver.h>

class CmdGameExportHandler : public ICmdHandler {
    public:
        CmdGameExportHandler();
        virtual QString cmd();
        virtual bool accessUnauthorized();
        virtual bool accessUser();
        virtual bool accessTester();
        virtual bool accessAdmin();
        virtual const QVector<CmdInputDef> &inputs();
        virtual QString description();
        virtual QStringList errors();
        virtual void handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj);

    private:
        QVector<CmdInputDef> m_vInputs;
        QString TAG;
};

#endif // CMD_GAME_EXPORT_HANDLER_H
