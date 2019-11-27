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

class CmdHandlerChatReadMessage : public CmdHandlerBase {
    public:
        CmdHandlerChatReadMessage();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatShowDialogs : public CmdHandlerBase {
    public:
        CmdHandlerChatShowDialogs();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatEditMessage : public CmdHandlerBase {
    public:
        CmdHandlerChatEditMessage();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatDeleteMessage : public CmdHandlerBase {
    public:
        CmdHandlerChatDeleteMessage();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatAddToChat : public CmdHandlerBase {
    public:
        CmdHandlerChatAddToChat();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatDeleteFromChat : public CmdHandlerBase {
    public:
        CmdHandlerChatDeleteFromChat();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatChangeOwner : public CmdHandlerBase {
    public:
        CmdHandlerChatChangeOwner();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatAddToBlackList : public CmdHandlerBase {
    public:
        CmdHandlerChatAddToBlackList();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatDeleteFromBlackList : public CmdHandlerBase {
    public:
        CmdHandlerChatDeleteFromBlackList();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatCreateGroupChat : public CmdHandlerBase {
    public:
        CmdHandlerChatCreateGroupChat();
        virtual void handle(ModelRequest *pRequest);
};


#endif // CMD_HANDLERS_COMMUNICATION_H
