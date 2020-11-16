#ifndef CMD_HADNLERS_MAILS_H
#define CMD_HADNLERS_MAILS_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerMailInfo : public CmdHandlerBase {

    public:
        CmdHandlerMailInfo();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerMailSend : public CmdHandlerBase {

    public:
        CmdHandlerMailSend();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerMailsList : public CmdHandlerBase {

    public:
        CmdHandlerMailsList();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HADNLERS_MAILS_H
