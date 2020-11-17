#ifndef CMD_HADNLERS_EVENTS_H
#define CMD_HADNLERS_EVENTS_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerEventAdd : public CmdHandlerBase {
    public:
        CmdHandlerEventAdd();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerEventDelete : public CmdHandlerBase {
    public:
        CmdHandlerEventDelete();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerEventInfo : public CmdHandlerBase {
    public:
        CmdHandlerEventInfo();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerEventsList : public CmdHandlerBase {
    public:
        CmdHandlerEventsList();
        virtual void handle(ModelRequest *pRequest);
};




#endif // CMD_HADNLERS_EVENTS_H
