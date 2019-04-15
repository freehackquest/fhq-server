#ifndef CMD_HADNLERS_USERS_H
#define CMD_HADNLERS_USERS_H

#include <cmd_handlers.h>

/*********************************************
 * This handler will return the list of leaks
**********************************************/

class CmdHandlerLeaksList : public CmdHandlerBase {
	
	public:
        CmdHandlerLeaksList();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerLeaksList)

/*********************************************
 * This handler will add one new leak
**********************************************/

class CmdHandlerLeaksAdd : public CmdHandlerBase {

    public:
        CmdHandlerLeaksAdd();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerLeaksAdd)

/*********************************************
 * This handler will update a leak
**********************************************/

class CmdHandlerLeaksUpdate : public CmdHandlerBase {

    public:
        CmdHandlerLeaksUpdate();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerLeaksUpdate)

/*********************************************
 * This handler will delete a leak
**********************************************/

class CmdHandlerLeaksDelete : public CmdHandlerBase {

    public:
        CmdHandlerLeaksDelete();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerLeaksDelete)

/*********************************************
 * This handler allows you to buy a leak
**********************************************/

class CmdHandlerLeaksBuy : public CmdHandlerBase {

    public:
        CmdHandlerLeaksBuy();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerLeaksBuy)

#endif // CMD_HADNLERS_USERS_H
