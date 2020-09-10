#ifndef CMD_HADNLERS_EVENTS_H
#define CMD_HADNLERS_EVENTS_H

#include <wsjcpp_jsonrpc20.h>

/*!
 * This handler will be create public event
 * */
class CmdHandlerEventAdd : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerEventAdd();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

/*!
 * This handler will be delete public event
 * */
class CmdHandlerEventDelete : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerEventDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

/*!
 * This handler will be retrun info about public event
 * */
class CmdHandlerEventInfo : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerEventInfo();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

/*!
 * This handler will be retrun list of public events
 * */
class CmdHandlerEventsList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerEventsList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};


#endif // CMD_HADNLERS_EVENTS_H
