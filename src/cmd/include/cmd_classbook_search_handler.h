#ifndef CMD_CLASSBOOK_SEARCH_HANDLER
#define CMD_CLASSBOOK_SEARCH_HANDLER

#include <iserver.h>

class CmdClassbookSearchHandler : public ICmdHandler {

    public:
        CmdClassbookSearchHandler();
        virtual QString cmd();
        virtual bool accessUnauthorized();
        virtual bool accessUser();
        virtual bool accessTester();
        virtual bool accessAdmin();
        virtual const QVector<CmdInputDef> &inputs();
        virtual QString description();
        virtual QStringList errors();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        QVector<CmdInputDef> m_vInputs;
};

#endif // CMD_CLASSBOOK_SEARCH_HANDLER

