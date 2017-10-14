#ifndef CMD_REGISTRATION_HANDLER_H
#define CMD_REGISTRATION_HANDLER_H

#include <iserver.h>

class CmdRegistrationHandler : public ICmdHandler {
	
	public:
        CmdRegistrationHandler();
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

#endif // CMD_REGISTRATION_HANDLER_H
