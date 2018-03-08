#ifndef CMD_MAILS_LIST_HANDLER_H
#define CMD_MAILS_LIST_HANDLER_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdMailsListHandler : public ICmdHandler {
	
	public:
        CmdMailsListHandler();
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

// RegistryCmdHandler<CmdMailsListHandler> registryCmdHandlerCmdMailsListHandler;

REGISTRY_CMD(CmdMailsListHandler)

#endif // CMD_MAILS_LIST_HANDLER_H
