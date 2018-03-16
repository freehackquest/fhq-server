#ifndef CMD_HANDLER_QUESTS_SUBJECTS_H
#define CMD_HANDLER_QUESTS_SUBJECTS_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdHandlerQuestsSubjects : public ICmdHandler {
	
	public:
        CmdHandlerQuestsSubjects();
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

REGISTRY_CMD(CmdHandlerQuestsSubjects)

#endif // CMD_HANDLER_QUESTS_SUBJECTS_H
