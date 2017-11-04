#ifndef CMD_CLASSBOOK_PROPOSAL_LIST_HANDLER_H
#define CMD_CLASSBOOK_PROPOSAL_LIST_HANDLER_H

#include <iserver.h>

class CmdClassbookProposalListHandler : public ICmdHandler {

    public:
        CmdClassbookProposalListHandler();
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
        QString TAG;
        QVector<CmdInputDef> m_vInputs;
};

#endif // CMD_CLASSBOOK_PROPOSAL_LIST_HANDLER_H

