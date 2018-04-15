#ifndef CMD_HADNLERS_GAMES_H
#define CMD_HADNLERS_GAMES_H

#include <cmd_handlers.h>

/*********************************************
 * Create Game
**********************************************/

class CmdHandlerGameCreate : public CmdHandlerBase {

    public:
        CmdHandlerGameCreate();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGameCreate)

/*********************************************
 * Delete Game
**********************************************/

class CmdHandlerGameDelete : public CmdHandlerBase {

    public:
        CmdHandlerGameDelete();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGameDelete)

/*********************************************
 * Export Game
**********************************************/

class CmdHandlerGameExport : public CmdHandlerBase {
    public:
        CmdHandlerGameExport();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGameExport)

/*********************************************
 * Import Game
**********************************************/

class CmdHandlerGameImport : public CmdHandlerBase {

    public:
        CmdHandlerGameImport();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGameImport)

/*********************************************
 * Info Game
**********************************************/

class CmdHandlerGameInfo : public CmdHandlerBase {

    public:
        CmdHandlerGameInfo();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGameInfo)

/*********************************************
 * Update Game
**********************************************/

class CmdHandlerGameUpdate : public CmdHandlerBase {

    public:
        CmdHandlerGameUpdate();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGameUpdate)

/*********************************************
 * Update Logo Game
**********************************************/

class CmdHandlerGameUpdateLogo : public CmdHandlerBase {

    public:
        CmdHandlerGameUpdateLogo();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGameUpdateLogo)

/*********************************************
 * List of Games
**********************************************/

class CmdHandlerGames : public CmdHandlerBase {

    public:
        CmdHandlerGames();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerGames)

#endif // CMD_HADNLERS_GAMES_H
