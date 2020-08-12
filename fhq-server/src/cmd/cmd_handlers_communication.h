#ifndef CMD_HANDLERS_COMMUNICATION_H
#define CMD_HANDLERS_COMMUNICATION_H

#include <cmd_handlers.h>

/*********************************************
 * Users chat
**********************************************/

class CmdHandlerChatSendMessage : public CmdHandlerBase {
    public:
        CmdHandlerChatSendMessage();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatSendMessage_new : public CmdHandlerBase {
    public:
        CmdHandlerChatSendMessage_new();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatLastestMessages : public CmdHandlerBase {
    public:
        CmdHandlerChatLastestMessages();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatReadMessage : public CmdHandlerBase {
    public:
        CmdHandlerChatReadMessage();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatShowDialogs : public CmdHandlerBase {
    public:
        CmdHandlerChatShowDialogs();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatEditMessage : public CmdHandlerBase {
    public:
        CmdHandlerChatEditMessage();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatDeleteMessage : public CmdHandlerBase {
    public:
        CmdHandlerChatDeleteMessage();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatAddToChat : public CmdHandlerBase {
    public:
        CmdHandlerChatAddToChat();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatDeleteFromChat : public CmdHandlerBase {
    public:
        CmdHandlerChatDeleteFromChat();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatChangeOwner : public CmdHandlerBase {
    public:
        CmdHandlerChatChangeOwner();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatAddToBlackList : public CmdHandlerBase {
    public:
        CmdHandlerChatAddToBlackList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatDeleteFromBlackList : public CmdHandlerBase {
    public:
        CmdHandlerChatDeleteFromBlackList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatCreateGroupChat : public CmdHandlerBase {
    public:
        CmdHandlerChatCreateGroupChat();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};


#endif // CMD_HANDLERS_COMMUNICATION_H
