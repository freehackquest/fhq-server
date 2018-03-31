#ifndef CMD_HANDLER_QUEST_H
#define CMD_HANDLER_QUEST_H

#include <cmd_handler_base.h>

class CmdHandlerQuest : public CmdHandlerBase {
	
	public:
		CmdHandlerQuest();
        virtual void handle(ModelRequest *pRequest);

};

REGISTRY_CMD(CmdHandlerQuest)

#endif // CMD_HANDLER_QUEST_H
