#ifndef CMD_HANDLER_SERVER_API_H
#define CMD_HANDLER_SERVER_API_H

#include <iserver.h>
#include <cmd_handlers.h>

/*! 
 * This handler will be return list of handlers - publish api interfaces
 * */
 
class CmdHandlerServerApi : public ICmdHandler {
	
	public:
        CmdHandlerServerApi();
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

REGISTRY_CMD(CmdHandlerServerApi)

#endif // CMD_HANDLER_SERVER_API_H
