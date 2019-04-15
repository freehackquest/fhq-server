#ifndef CMD_HADNLERS_SUPPORT_H
#define CMD_HADNLERS_SUPPORT_H

#include <cmd_handlers.h>

/*!
 * This handler for add support
 * */
class CmdHandlerFeedbackAdd : public CmdHandlerBase {

    public:
        CmdHandlerFeedbackAdd();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerFeedbackAdd)

#endif // CMD_HADNLERS_SUPPORT_H
