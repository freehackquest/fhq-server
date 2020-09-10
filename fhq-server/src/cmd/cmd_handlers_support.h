#ifndef CMD_HADNLERS_SUPPORT_H
#define CMD_HADNLERS_SUPPORT_H

#include <wsjcpp_jsonrpc20.h>

/*!
 * This handler for add support
 * */
class CmdHandlerFeedbackAdd : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerFeedbackAdd();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerFeedbackAdd)

#endif // CMD_HADNLERS_SUPPORT_H
