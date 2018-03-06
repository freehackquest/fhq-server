#ifndef CMD_CLASSBOOK_EXPORT
#define CMD_CLASSBOOK_EXPORT

#include <iserver.h>
#include <QFile>

class CmdClassbookExportHandler : public ICmdHandler {

    public:
        CmdClassbookExportHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;

        void createHtml(QFile *file, QString lang, QSqlQuery query);
        void createMD(QFile *file , QString lang, QSqlQuery query);
};

#endif // CMD_CLASSBOOK_EXPORT

