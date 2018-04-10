#ifndef CMD_HADNLERS_USERS_H
#define CMD_HADNLERS_USERS_H

#include <cmd_handler_base.h>

/*!
 * This handler will be return scoreboard of user
 * */
class CmdHandlerUsersScoreboard : public CmdHandlerBase {
	
	public:
		CmdHandlerUsersScoreboard();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUsersScoreboard)

#endif // CMD_HADNLERS_USERS_H
