#ifndef CMD_HANDLERS_COMMUNICATION_H
#define CMD_HANDLERS_COMMUNICATION_H

#include <wsjcpp_jsonrpc20.h>

/*********************************************
 * Users chat
**********************************************/

class CmdHandlerChatSendMessage : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatSendMessage();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatSendMessage_new : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatSendMessage_new();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatLastestMessages : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatLastestMessages();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatReadMessage : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatReadMessage();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatShowDialogs : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatShowDialogs();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatEditMessage : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatEditMessage();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatDeleteMessage : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatDeleteMessage();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatAddToChat : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatAddToChat();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatDeleteFromChat : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatDeleteFromChat();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatChangeOwner : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatChangeOwner();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatAddToBlackList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatAddToBlackList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatDeleteFromBlackList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatDeleteFromBlackList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerChatCreateGroupChat : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerChatCreateGroupChat();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};


#endif // CMD_HANDLERS_COMMUNICATION_H
