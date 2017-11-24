#ifndef CMD_USER_SKILLS_HANDLER_H
#define CMD_USER_SKILLS_HANDLER_H

#include <iserver.h>

class CmdUserSkillsHandler : public ICmdHandler {
	
	public:
		CmdUserSkillsHandler();
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

#endif // CMD_USER_SKILLS_HANDLER_H
