#ifndef CMD_HADNLERS_SERVER_H
#define CMD_HADNLERS_SERVER_H

#include <cmd_handler_base.h>

/*!
 * This handler will be return list of handlers - publish api interfaces
 * */

class CmdHandlerServerApi : public ICmdHandler {

    public:
        CmdHandlerServerApi();
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

REGISTRY_CMD(CmdHandlerServerApi)

/*!
 * This handler will be return public info
 * */

class CmdPublicInfoHandler : public ICmdHandler {

    public:
        CmdPublicInfoHandler();
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

REGISTRY_CMD(CmdPublicInfoHandler)


/*!
 * This handler will be return private server info
 * */

class CmdServerInfoHandler : public ICmdHandler {

    public:
        CmdServerInfoHandler();
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

REGISTRY_CMD(CmdServerInfoHandler)

/*!
 * This handler will be return private server settings
 * */

class CmdServerSettingsHandler : public ICmdHandler {

    public:
        CmdServerSettingsHandler();
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

REGISTRY_CMD(CmdServerSettingsHandler)

/*!
 * This handler for update private server settings
 * */

class CmdServerSettingsUpdateHandler : public ICmdHandler {

    public:
        CmdServerSettingsUpdateHandler();
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

REGISTRY_CMD(CmdServerSettingsUpdateHandler)

#endif // CMD_HADNLERS_SERVER_H
