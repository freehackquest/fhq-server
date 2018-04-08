#ifndef CMD_HANDLERS_QUESTS_H
#define CMD_HANDLERS_QUESTS_H

#include <cmd_handler_base.h>

/*********************************************
 * List of Quests
**********************************************/

class CmdHandlerQuests : public CmdHandlerBase {
	
	public:
        CmdHandlerQuests();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuests)

/*********************************************
 * Quest Info
**********************************************/

class CmdHandlerQuest : public CmdHandlerBase {

    public:
        CmdHandlerQuest();
        virtual void handle(ModelRequest *pRequest);

};

REGISTRY_CMD(CmdHandlerQuest)

#endif // CMD_HANDLERS_QUESTS_H
