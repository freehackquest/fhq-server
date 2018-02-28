#ifndef CMD_CLASSBOOK_LOCALIZATION_ADD_RECORD_H
#define CMD_CLASSBOOK_LOCALIZATION_ADD_RECORD_H

#include <iserver.h>

class CmdClassbookLocalizationAddRecordHandler : public ICmdHandler {

    public:
        CmdClassbookLocalizationAddRecordHandler();
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
        QString TAG;
        std::vector<CmdInputDef> m_vInputs;
};

#endif // CMD_CLASSBOOK_LOCALIZATION_ADD_RECORD_H

