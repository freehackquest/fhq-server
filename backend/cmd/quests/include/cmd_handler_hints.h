#ifndef CMD_HANDLER_HINTS_H
#define CMD_HANDLER_HINTS_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdHandlerHints : public ICmdHandler {
	
	public:
		CmdHandlerHints();
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

REGISTRY_CMD(CmdHandlerHints)

#endif // CMD_HANDLER_HINTS_H
