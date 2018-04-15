#ifndef CMD_HADNLERS_SERVER_H
#define CMD_HADNLERS_SERVER_H

#include <cmd_handlers.h>

/*!
 * This handler will be return list of handlers - publish api interfaces
 * */

class CmdHandlerServerApi : public CmdHandlerBase {

    public:
        CmdHandlerServerApi();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerServerApi)

/*!
 * This handler will be return public info
 * */

class CmdHandlerPublicInfo : public CmdHandlerBase {

    public:
        CmdHandlerPublicInfo();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerPublicInfo)


/*!
 * This handler will be return private server info
 * */

class CmdHandlerServerInfo : public CmdHandlerBase {

    public:
        CmdHandlerServerInfo();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerServerInfo)

/*!
 * This handler will be return private server settings
 * */

class CmdHandlerServerSettings : public CmdHandlerBase {

    public:
        CmdHandlerServerSettings();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerServerSettings)

/*!
 * This handler for update private server settings
 * */

class CmdHandlerServerSettingsUpdate : public CmdHandlerBase {

    public:
        CmdHandlerServerSettingsUpdate();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerServerSettingsUpdate)

#endif // CMD_HADNLERS_SERVER_H
