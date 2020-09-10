#ifndef CMD_HADNLERS_SERVER_H
#define CMD_HADNLERS_SERVER_H

#include <wsjcpp_jsonrpc20.h>

/*!
 * This handler will be return public info
 * */

class CmdHandlerPublicInfo : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerPublicInfo();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerPublicInfo)


/*!
 * This handler will be return private server info
 * */

class CmdHandlerServerInfo : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerServerInfo();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerServerInfo)

/*!
 * This handler will be return private server settings
 * */

class CmdHandlerServerSettings : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerServerSettings();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerServerSettings)

/*!
 * This handler for update private server settings
 * */

class CmdHandlerServerSettingsUpdate : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerServerSettingsUpdate();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerServerSettingsUpdate)

#endif // CMD_HADNLERS_SERVER_H
