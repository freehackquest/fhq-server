#ifndef CMD_HADNLERS_GAMES_H
#define CMD_HADNLERS_GAMES_H

#include <cmd_handler_base.h>

/*********************************************
 * Create Game
**********************************************/

class CmdGameCreateHandler : public CmdHandlerBase {

    public:
        CmdGameCreateHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGameCreateHandler)

/*********************************************
 * Delete Game
**********************************************/

class CmdGameDeleteHandler : public CmdHandlerBase {

    public:
        CmdGameDeleteHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGameDeleteHandler)

/*********************************************
 * Export Game
**********************************************/

class CmdGameExportHandler : public CmdHandlerBase {
    public:
        CmdGameExportHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGameExportHandler)

/*********************************************
 * Import Game
**********************************************/

class CmdGameImportHandler : public CmdHandlerBase {

    public:
        CmdGameImportHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGameImportHandler)

/*********************************************
 * Info Game
**********************************************/

class CmdGameInfoHandler : public CmdHandlerBase {

    public:
        CmdGameInfoHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGameInfoHandler)

/*********************************************
 * Update Game
**********************************************/

class CmdGameUpdateHandler : public CmdHandlerBase {

    public:
        CmdGameUpdateHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGameUpdateHandler)

/*********************************************
 * Update Logo Game
**********************************************/

class CmdGameUpdateLogoHandler : public CmdHandlerBase {

    public:
        CmdGameUpdateLogoHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGameUpdateLogoHandler)

/*********************************************
 * List of Games
**********************************************/

class CmdGamesHandler : public CmdHandlerBase {

    public:
        CmdGamesHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdGamesHandler)

#endif // CMD_HADNLERS_GAMES_H
