#ifndef CMD_HADNLERS_USERS_H
#define CMD_HADNLERS_USERS_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerLeaksList : public CmdHandlerBase {

    public:
        CmdHandlerLeaksList();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerLeaksAdd : public CmdHandlerBase {

    public:
        CmdHandlerLeaksAdd();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerLeaksUpdate : public CmdHandlerBase {

    public:
        CmdHandlerLeaksUpdate();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerLeaksDelete : public CmdHandlerBase {

    public:
        CmdHandlerLeaksDelete();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerLeaksBuy : public CmdHandlerBase {

    public:
        CmdHandlerLeaksBuy();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HADNLERS_USERS_H
