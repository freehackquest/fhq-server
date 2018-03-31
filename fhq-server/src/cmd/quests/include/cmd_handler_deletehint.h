#ifndef CMD_HANDLER_DELETEHINT_H
#define CMD_HANDLER_DELETEHINT_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdHandlerDeleteHint : public ICmdHandler {
	
	public:
		CmdHandlerDeleteHint();
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

REGISTRY_CMD(CmdHandlerDeleteHint)

#endif // CMD_HANDLER_DELETEHINT_H
