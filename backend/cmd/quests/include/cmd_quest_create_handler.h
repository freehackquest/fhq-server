#ifndef CMD_QUEST_CREATE_HANDLER_H
#define CMD_QUEST_CREATE_HANDLER_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdCreateQuestHandler : public ICmdHandler {
	
	public:
		CmdCreateQuestHandler();
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

#endif // CMD_QUEST_CREATE_HANDLER_H
