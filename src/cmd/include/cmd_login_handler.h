#ifndef CMD_LOGIN_HANDLER_H
#define CMD_LOGIN_HANDLER_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdLoginHandler : public ICmdHandler {
	
	public:
		CmdLoginHandler();
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

#endif // CMD_LOGIN_HANDLER_H
