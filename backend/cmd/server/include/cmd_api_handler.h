#ifndef CMD_API_HANDLER_H
#define CMD_API_HANDLER_H

#include <iserver.h>
#include <cmd_handlers.h>

/*! 
 * This handler will be return list of handlers - publish api interfaces
 * */
 
class CmdApiHandler : public ICmdHandler {
	
	public:
		CmdApiHandler();
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

REGISTRY_CMD(CmdApiHandler)

#endif // CMD_API_HANDLER_H
