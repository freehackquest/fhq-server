#ifndef CMD_HANDLERS_COMMUNICATION_H
#define CMD_HANDLERS_COMMUNICATION_H

#include <cmd_handlers.h>

/*********************************************
 * Users chat
**********************************************/

class CmdHandlerChatSendMessage : public CmdHandlerBase {
    public:
        CmdHandlerChatSendMessage();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatLastestMessages : public CmdHandlerBase {
    public:
        CmdHandlerChatLastestMessages();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HANDLERS_COMMUNICATION_H
