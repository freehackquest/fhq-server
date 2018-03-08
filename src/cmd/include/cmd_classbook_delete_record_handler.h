#ifndef CMD_CLASSBOOK_DELETE_RECORD_HANDLER
#define CMD_CLASSBOOK_DELETE_RECORD_HANDLER

#include <iserver.h>
#include <cmd_handlers.h>

class CmdClassbookDeleteRecordHandler : public ICmdHandler {

    public:
        CmdClassbookDeleteRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

#endif // CMD_CLASSBOOK_DELETE_RECORD_HANDLER

