#ifndef CMD_HADNLERS_USERS_H
#define CMD_HADNLERS_USERS_H

#include <cmd_handler_base.h>

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
class CmdGetMapHandler : public ICmdHandler {

    public:
        CmdGetMapHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdGetMapHandler)

/*!
 * User login
 * */
class CmdLoginHandler : public ICmdHandler {

    public:
        CmdLoginHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdLoginHandler)

/*!
 * User registration
 * */

class CmdRegistrationHandler : public ICmdHandler {

    public:
        CmdRegistrationHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdRegistrationHandler)


/*!
 * Users chat
 * */

class CmdSendChatMessageHandler : public ICmdHandler {

    public:
        CmdSendChatMessageHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdSendChatMessageHandler)

/*!
 * User login by token
 * */

class CmdTokenHandler : public ICmdHandler {

    public:
        CmdTokenHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdTokenHandler)

/*!
 * User location update
 * */

class CmdUpdateUserLocationHandler : public ICmdHandler {

    public:
        CmdUpdateUserLocationHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdUpdateUserLocationHandler)

/*!
 * User change password
 * */

class CmdUserChangePasswordHandler : public ICmdHandler {

    public:
        CmdUserChangePasswordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdUserChangePasswordHandler)

/*!
 * User create
 * */

class CmdUserCreateHandler : public ICmdHandler {

    public:
        CmdUserCreateHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdUserCreateHandler)

/*!
 * User info
 * */

class CmdUserHandler : public ICmdHandler {

    public:
        CmdUserHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdUserHandler)

/*!
 * User reset password
 * */

class CmdUserResetPasswordHandler : public ICmdHandler {

    public:
        CmdUserResetPasswordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdUserResetPasswordHandler)

/*!
 * User skill
 * */

class CmdUserSkillsHandler : public ICmdHandler {

    public:
        CmdUserSkillsHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdUserSkillsHandler)

/*!
 * User update
 * */

class CmdUserUpdateHandler : public ICmdHandler {

    public:
        CmdUserUpdateHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdUserUpdateHandler)

/*!
 * Users
 * */

class CmdUsersHandler : public ICmdHandler {

    public:
        CmdUsersHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdUsersHandler)

#endif // CMD_HADNLERS_USERS_H
