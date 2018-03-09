#ifndef CMD_CLASSBOOK_GET_INFO_HANDLER
#define CMD_CLASSBOOK_GET_INFO_HANDLER

#include <iserver.h>
#include <cmd_handlers.h>

class CmdClassbookInfoHandler : public ICmdHandler {

    public:
        CmdClassbookInfoHandler();
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

REGISTRY_CMD(CmdClassbookInfoHandler)

#endif // CMD_CLASSBOOK_GET_INFO_HANDLER

