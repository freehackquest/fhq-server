#ifndef CMD_CLASSBOOK_UPDATE_RECORD_HANDLER
#define CMD_CLASSBOOK_UPDATE_RECORD_HANDLER

#include <iserver.h>

class CmdClassbookUpdateRecordHandler : public ICmdHandler {

    public:
        CmdClassbookUpdateRecordHandler();
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

#endif // CMD_CLASSBOOK_UPDATE_RECORD_HANDLER

