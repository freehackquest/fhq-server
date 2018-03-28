#ifndef CMD_HANDLER_QUEST_STATISTICS_H
#define CMD_HANDLER_QUEST_STATISTICS_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdHandlerQuestStatistics : public ICmdHandler {
	
	public:
        CmdHandlerQuestStatistics();
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

REGISTRY_CMD(CmdHandlerQuestStatistics)

#endif // CMD_HANDLER_QUEST_STATISTICS_H
