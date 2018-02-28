#ifndef CMD_CLASSBOOK_EXPORT
#define CMD_CLASSBOOK_EXPORT

#include <iserver.h>
#include <QFile>

class CmdClassbookExportHandler : public ICmdHandler {

    public:
        CmdClassbookExportHandler();
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
        void createHtml(QFile *file, QString lang, QSqlQuery query);
        void createMD(QFile *file , QString lang, QSqlQuery query);
};

#endif // CMD_CLASSBOOK_EXPORT

