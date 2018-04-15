#ifndef CMD_HADNLERS_USERS_H
#define CMD_HADNLERS_USERS_H

#include <cmd_handlers.h>

/*!
 * This handler will be return scoreboard of user
 * */
class CmdHandlerUsersScoreboard : public CmdHandlerBase {
	
	public:
		CmdHandlerUsersScoreboard();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerUsersScoreboard)

/*!
 * This handler will be return json map users
 * */
class CmdGetMapHandler : public CmdHandlerBase {
    public:
        CmdGetMapHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGetMapHandler)

/*!
 * User login
 * */
class CmdLoginHandler : public CmdHandlerBase {
    public:
        CmdLoginHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdLoginHandler)

/*!
 * User registration
 * */
class CmdRegistrationHandler : public CmdHandlerBase {
    public:
        CmdRegistrationHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdRegistrationHandler)

/*!
 * Users chat
 * */
class CmdSendChatMessageHandler : public CmdHandlerBase {
    public:
        CmdSendChatMessageHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdSendChatMessageHandler)

/*!
 * User login by token
 * */
class CmdTokenHandler : public CmdHandlerBase {
    public:
        CmdTokenHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdTokenHandler)

/*!
 * User location update
 * */
class CmdUpdateUserLocationHandler : public CmdHandlerBase {
    public:
        CmdUpdateUserLocationHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUpdateUserLocationHandler)

/*!
 * User change password
 * */
class CmdUserChangePasswordHandler : public CmdHandlerBase {
    public:
        CmdUserChangePasswordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUserChangePasswordHandler)

/*!
 * User create
 * */
class CmdUserCreateHandler : public CmdHandlerBase {
    public:
        CmdUserCreateHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUserCreateHandler)

/*!
 * User info
 * */
class CmdUserHandler : public CmdHandlerBase {
    public:
        CmdUserHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUserHandler)

/*!
 * User reset password
 * */
class CmdUserResetPasswordHandler : public CmdHandlerBase {
    public:
        CmdUserResetPasswordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUserResetPasswordHandler)

/*!
 * User skill
 * */

class CmdUserSkillsHandler : public CmdHandlerBase {

    public:
        CmdUserSkillsHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUserSkillsHandler)

/*!
 * User update
 * */
class CmdUserUpdateHandler : public CmdHandlerBase {
    public:
        CmdUserUpdateHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUserUpdateHandler)

/*!
 * Users
 * */
class CmdUsersHandler : public CmdHandlerBase {
    public:
        CmdUsersHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdUsersHandler)

#endif // CMD_HADNLERS_USERS_H
