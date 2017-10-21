#ifndef CMD_GAME_UPDATE_LOGO_HANDLER_H
#define CMD_GAME_UPDATE_LOGO_HANDLER_H

#include <iserver.h>

class CmdGameUpdateLogoHandler : public ICmdHandler {
	
    public:
        CmdGameUpdateLogoHandler();
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
};

#endif // CMD_GAME_UPDATE_LOGO_HANDLER_H
