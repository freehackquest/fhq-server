#ifndef CMD_CLASSBOOK_GETLIST_HANDLER
#define CMD_CLASSBOOK_GETLIST_HANDLER

#include <iserver.h>
#include <cmd_handlers.h>

class CmdClassbookListHandler : public ICmdHandler {

    public:
        CmdClassbookListHandler();
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

REGISTRY_CMD(CmdClassbookListHandler)

#endif // CMD_CLASSBOOK_GETLIST_HANDLER

