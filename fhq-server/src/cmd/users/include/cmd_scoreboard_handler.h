#ifndef CMD_SCOREBOARD_HANDLER_H
#define CMD_SCOREBOARD_HANDLER_H

#include <iserver.h>
#include <cmd_handler_base.h>

/*********************************************
 * User Scoreboard
**********************************************/

class CmdScoreboardHandler : public CmdHandlerBase {
	
	public:
		CmdScoreboardHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdScoreboardHandler)

#endif // CMD_SCOREBOARD_HANDLER_H
