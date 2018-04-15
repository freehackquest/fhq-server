#ifndef CMD_USER_DELETE_HANDLER_H
#define CMD_USER_DELETE_HANDLER_H

#include <iserver.h>
#include <cmd_handler_base.h>

class CmdUserDeleteHandler : public CmdHandlerBase {
    public:
        CmdUserDeleteHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUserDeleteHandler)

#endif // CMD_USER_DELETE_HANDLER_H

