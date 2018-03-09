#ifndef CMD_SERVER_SETTINGS_HANDLER_H
#define CMD_SERVER_SETTINGS_HANDLER_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdServerSettingsHandler : public ICmdHandler {
	
	public:
        CmdServerSettingsHandler();
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

REGISTRY_CMD(CmdServerSettingsHandler)

#endif // CMD_SERVER_SETTINGS_HANDLER_H
