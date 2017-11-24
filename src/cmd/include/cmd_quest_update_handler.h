#ifndef CMD_QUEST_UPDATE_HANDLER_H
#define CMD_QUEST_UPDATE_HANDLER_H

#include <iserver.h>

class CmdQuestUpdateHandler : public ICmdHandler {
	
	public:
        CmdQuestUpdateHandler();
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

#endif // CMD_QUEST_UPDATE_HANDLER_H
