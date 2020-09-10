#ifndef CMD_HADNLERS_USERS_H
#define CMD_HADNLERS_USERS_H

#include <wsjcpp_jsonrpc20.h>

/*********************************************
 * This handler will return the list of leaks
**********************************************/

class CmdHandlerLeaksList : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerLeaksList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLeaksList)

/*********************************************
 * This handler will add one new leak
**********************************************/

class CmdHandlerLeaksAdd : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerLeaksAdd();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLeaksAdd)

/*********************************************
 * This handler will update a leak
**********************************************/

class CmdHandlerLeaksUpdate : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerLeaksUpdate();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLeaksUpdate)

/*********************************************
 * This handler will delete a leak
**********************************************/

class CmdHandlerLeaksDelete : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerLeaksDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLeaksDelete)

/*********************************************
 * This handler allows you to buy a leak
**********************************************/

class CmdHandlerLeaksBuy : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerLeaksBuy();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLeaksBuy)

#endif // CMD_HADNLERS_USERS_H
