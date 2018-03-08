#ifndef CMD_ADDHINT_HANDLER_H
#define CMD_ADDHINT_HANDLER_H

#include <iserver.h>
#include <model_request.h>
#include <cmd_handlers.h>

class CmdAddHintHandler : public ICmdHandler {
	
	public:
		CmdAddHintHandler();
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

#endif // CMD_ADDHINT_HANDLER_H
