#ifndef INTERFACES_ICMDHANDLER_H
#define INTERFACES_ICMDHANDLER_H

#include <QString>
#include <QJsonObject>
#include <QVector>
#include <iwebsocketserver.h>
#include "../server/headers/cmd_input_def.h"

class ICmdHandler {
	public:
		virtual QString cmd() = 0;
		virtual bool accessUnauthorized() = 0;
		virtual bool accessUser() = 0;
		virtual bool accessTester() = 0;
		virtual bool accessAdmin() = 0;
		virtual QString description() = 0;
		virtual const QVector<CmdInputDef> &inputs() = 0;
		virtual QStringList errors() = 0;
		virtual void handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj) = 0;
};

#endif // INTERFACES_ICMDHANDLER_H
