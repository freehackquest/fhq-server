#ifndef CMD_GETMAP_HANDLER_H
#define CMD_GETMAP_HANDLER_H

#include <iserver.h>
#include <QString>
#include <QVariant>

class CmdGetMapHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual bool accessUnauthorized();
		virtual bool accessUser();
		virtual bool accessTester();
		virtual bool accessAdmin();
		virtual QString short_description();
		virtual QString description();
		virtual QStringList errors();
		virtual void handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj);
};

#endif // CMD_GETMAP_HANDLER_H
