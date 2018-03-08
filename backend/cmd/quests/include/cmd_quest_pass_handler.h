#ifndef CMD_QUEST_PASS_HANDLER_H
#define CMD_QUEST_PASS_HANDLER_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdQuestPassHandler : public ICmdHandler {
	
	public:
        CmdQuestPassHandler();
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

REGISTRY_CMD(CmdQuestPassHandler)

#endif // CMD_QUEST_PASS_HANDLER_H
