#ifndef CMD_HADNLERS_USERS_H
#define CMD_HADNLERS_USERS_H

#include <wsjcpp_jsonrpc20.h>

/*********************************************
 * This handler will be return scoreboard of user
**********************************************/

class CmdHandlerUsersScoreboard : public WsjcppJsonRpc20HandlerBase {
    
    public:
        CmdHandlerUsersScoreboard();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUsersScoreboard)

/*********************************************
 * This handler will be return json map users
**********************************************/

class CmdHandlerGetMap : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerGetMap();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGetMap)

/*********************************************
 * User login
**********************************************/

class CmdHandlerLogin : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerLogin();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLogin)

/*********************************************
 * User registration
**********************************************/

class CmdHandlerRegistration : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerRegistration();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerRegistration)

/*********************************************
 * User login by token
**********************************************/

class CmdHandlerToken : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerToken();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerToken)

/*********************************************
 * User location update
**********************************************/

class CmdHandlerUpdateUserLocation : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUpdateUserLocation();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUpdateUserLocation)

/*********************************************
 * User change password
**********************************************/

class CmdHandlerUserChangePassword : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUserChangePassword();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUserChangePassword)

/*********************************************
 * Add new user
**********************************************/

class CmdHandlerUsersAdd : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsersAdd();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUsersAdd)

/*********************************************
 * User info (deprecated)
**********************************************/

class CmdHandlerUser : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUser();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUser)

/*********************************************
 * User info
**********************************************/

class CmdHandlerUsersInfo : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsersInfo();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUsersInfo)

/*********************************************
 * User reset password
**********************************************/

class CmdHandlerUserResetPassword : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUserResetPassword();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUserResetPassword)

/*********************************************
 * User skill
**********************************************/

class CmdHandlerUserSkills : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUserSkills();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUserSkills)

/*********************************************
 * User update
**********************************************/

class CmdHandlerUserUpdate : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUserUpdate();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUserUpdate)

/*********************************************
 * User delete
**********************************************/

class CmdHandlerUserDelete : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUserDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUserDelete)

/*********************************************
 * Users
**********************************************/
 
class CmdHandlerUsers : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsers();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUsers)

/*********************************************
 * User registration
**********************************************/

class CmdHandlerUsersRegistration : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsersRegistration();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUsersRegistration)

/*********************************************
 * User registration verification 
**********************************************/

class CmdHandlerUsersRegistrationVerification : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsersRegistrationVerification();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUsersRegistrationVerification)

/*********************************************
 * Change user's email
**********************************************/
 
class CmdHandlerUsersChangeEmail : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsersChangeEmail();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerUsersChangeEmail)

/*********************************************
 * Change user's email verification
**********************************************/
 
class CmdHandlerUsersChangeEmailVerification : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsersChangeEmailVerification();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};


/*********************************************
 * List of user tokens
**********************************************/
 
class CmdHandlerUsersTokens : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsersTokens();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};


/*********************************************
 * Delete user tokens
**********************************************/
 
class CmdHandlerUsersTokensDelete : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsersTokensDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

#endif // CMD_HADNLERS_USERS_H
