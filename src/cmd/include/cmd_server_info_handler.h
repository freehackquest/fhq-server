#ifndef CMD_SERVERINFO_HANDLER_H
#define CMD_SERVERINFO_HANDLER_H

#include <iserver.h>

class CmdServerInfoHandler : public ICmdHandler {
	
	public:
		CmdServerInfoHandler();
        virtual std::string cmd();
		virtual bool accessUnauthorized();
		virtual bool accessUser();
		virtual bool accessTester();
		virtual bool accessAdmin();
        virtual const std::vector<CmdInputDef> &inputs();
		virtual QString description();
		virtual QStringList errors();
		virtual void handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj);
		
	private:
        std::vector<CmdInputDef> m_vInputs;
        QString TAG;
};

#endif // CMD_SERVERINFO_HANDLER_H
