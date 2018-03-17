#ifndef CMD_HANDLER_ADDHINT_H
#define CMD_HANDLER_ADDHINT_H

#include <iserver.h>
#include <model_request.h>
#include <cmd_handlers.h>

class CmdHandlerAddHint : public ICmdHandler {
	
	public:
		CmdHandlerAddHint();
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

REGISTRY_CMD(CmdHandlerAddHint)

#endif // CMD_HANDLER_ADDHINT_H
