#ifndef CMD_CLASSBOOK_UPDATE_RECORD_HANDLER
#define CMD_CLASSBOOK_UPDATE_RECORD_HANDLER

#include <iserver.h>

class CmdClassbookUpdateRecordHandler : public ICmdHandler {

    public:
        CmdClassbookUpdateRecordHandler();
        virtual std::string cmd();
        virtual bool accessUnauthorized();
        virtual bool accessUser();
        virtual bool accessTester();
        virtual bool accessAdmin();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual QString description();
        virtual QStringList errors();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        std::vector<CmdInputDef> m_vInputs;
};

#endif // CMD_CLASSBOOK_UPDATE_RECORD_HANDLER

