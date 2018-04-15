#ifndef CMD_HADNLERS_EVENTS_H
#define CMD_HADNLERS_EVENTS_H

#include <cmd_handlers.h>

/*!
 * This handler will be create public event
 * */
class CmdHandlerEventAdd : public CmdHandlerBase {
    public:
        CmdHandlerEventAdd();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerEventAdd)

/*!
 * This handler will be delete public event
 * */
class CmdHandlerEventDelete : public CmdHandlerBase {
    public:
        CmdHandlerEventDelete();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerEventDelete)

/*!
 * This handler will be retrun info about public event
 * */
class CmdHandlerEventInfo : public CmdHandlerBase {
    public:
        CmdHandlerEventInfo();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerEventInfo)

/*!
 * This handler will be retrun list of public events
 * */
class CmdHandlerEventsList : public CmdHandlerBase {
    public:
        CmdHandlerEventsList();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerEventsList)


#endif // CMD_HADNLERS_EVENTS_H
