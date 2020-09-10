#ifndef CMD_HADNLERS_MAILS_H
#define CMD_HADNLERS_MAILS_H

#include <wsjcpp_jsonrpc20.h>

/*!
 * This handler will be return info about mail
 * */

class CmdHandlerMailInfo : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerMailInfo();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerMailInfo)

/*!
 * This handler will be send mail by admin
 * */

class CmdHandlerMailSend : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerMailSend();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerMailSend)

/*!
 * This handler will be list of mails
 * */

class CmdHandlerMailsList : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerMailsList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerMailsList)

#endif // CMD_HADNLERS_MAILS_H
