#ifndef CMD_HANDLER_QUEST_PASS_H
#define CMD_HANDLER_QUEST_PASS_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdHandlerQuestPass : public ICmdHandler {
	
	public:
        CmdHandlerQuestPass();
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

REGISTRY_CMD(CmdHandlerQuestPass)

#endif // CMD_HANDLER_QUEST_PASS_HANDLER_H
