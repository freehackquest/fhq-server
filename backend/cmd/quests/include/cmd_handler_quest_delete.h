#ifndef CMD_HANDLER_QUEST_DELETE_H
#define CMD_HANDLER_QUEST_DELETE_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdHandlerQuestDelete : public ICmdHandler {
	
	public:
        CmdHandlerQuestDelete();
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

REGISTRY_CMD(CmdHandlerQuestDelete)

#endif // CMD_HANDLER_QUEST_DELETE_H
