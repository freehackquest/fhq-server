#ifndef CMD_HADNLERS_USERS_H
#define CMD_HADNLERS_USERS_H

#include <cmd_handlers.h>

/*********************************************
 * This handler will be return scoreboard of user
**********************************************/

class CmdHandlerUsersScoreboard : public CmdHandlerBase {
	
	public:
		CmdHandlerUsersScoreboard();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUsersScoreboard)

/*********************************************
 * This handler will be return json map users
**********************************************/

class CmdHandlerGetMap : public CmdHandlerBase {
    public:
        CmdHandlerGetMap();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGetMap)

/*********************************************
 * User login
**********************************************/

class CmdHandlerLogin : public CmdHandlerBase {
    public:
        CmdHandlerLogin();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerLogin)

/*********************************************
 * User registration
**********************************************/

class CmdHandlerRegistration : public CmdHandlerBase {
    public:
        CmdHandlerRegistration();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerRegistration)

/*********************************************
 * Users chat
**********************************************/

class CmdHandlerSendChatMessage : public CmdHandlerBase {
    public:
        CmdHandlerSendChatMessage();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerSendChatMessage)

/*********************************************
 * User login by token
**********************************************/

class CmdHandlerToken : public CmdHandlerBase {
    public:
        CmdHandlerToken();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerToken)

/*********************************************
 * User location update
**********************************************/

class CmdHandlerUpdateUserLocation : public CmdHandlerBase {
    public:
        CmdHandlerUpdateUserLocation();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUpdateUserLocation)

/*********************************************
 * User change password
**********************************************/

class CmdHandlerUserChangePassword : public CmdHandlerBase {
    public:
        CmdHandlerUserChangePassword();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUserChangePassword)

/*********************************************
 * User create
**********************************************/

class CmdHandlerUserCreate : public CmdHandlerBase {
    public:
        CmdHandlerUserCreate();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUserCreate)

/*********************************************
 * User info
**********************************************/

class CmdHandlerUser : public CmdHandlerBase {
    public:
        CmdHandlerUser();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUser)

/*********************************************
 * User reset password
**********************************************/

class CmdHandlerUserResetPassword : public CmdHandlerBase {
    public:
        CmdHandlerUserResetPassword();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUserResetPassword)

/*********************************************
 * User skill
**********************************************/

class CmdHandlerUserSkills : public CmdHandlerBase {

    public:
        CmdHandlerUserSkills();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUserSkills)

/*********************************************
 * User update
**********************************************/

class CmdHandlerUserUpdate : public CmdHandlerBase {
    public:
        CmdHandlerUserUpdate();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUserUpdate)

/*********************************************
 * User delete
**********************************************/

class CmdHandlerUserDelete : public CmdHandlerBase {
    public:
        CmdHandlerUserDelete();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUserDelete)

/*********************************************
 * Users
**********************************************/
 
class CmdHandlerUsers : public CmdHandlerBase {
    public:
        CmdHandlerUsers();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUsers)

#endif // CMD_HADNLERS_USERS_H
