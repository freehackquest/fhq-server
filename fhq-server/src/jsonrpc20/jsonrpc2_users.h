#ifndef JSONRPC2_USERS_H
#define JSONRPC2_USERS_H

#include <cmd_handlers.h>

/*********************************************
 * User Retrive
**********************************************/

class Jsonrpc2UserRetrive : public WsjcppJsonrpc2Base {
    public:
        Jsonrpc2UserRetrive();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_WSJCPP_JSONRPC2(Jsonrpc2UserRetrive)

#endif // JSONRPC2_USERS_H
