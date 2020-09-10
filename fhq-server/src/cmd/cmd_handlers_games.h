#ifndef CMD_HADNLERS_GAMES_H
#define CMD_HADNLERS_GAMES_H

#include <wsjcpp_jsonrpc20.h>

/*********************************************
 * Create Game
**********************************************/

class CmdHandlerGameCreate : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerGameCreate();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGameCreate)

/*********************************************
 * Delete Game
**********************************************/

class CmdHandlerGameDelete : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerGameDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGameDelete)

/*********************************************
 * Export Game
**********************************************/

class CmdHandlerGameExport : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerGameExport();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGameExport)

/*********************************************
 * Import Game
**********************************************/

class CmdHandlerGameImport : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerGameImport();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGameImport)

/*********************************************
 * Info Game
**********************************************/

class CmdHandlerGameInfo : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerGameInfo();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGameInfo)

/*********************************************
 * Update Game
**********************************************/

class CmdHandlerGameUpdate : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerGameUpdate();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGameUpdate)

/*********************************************
 * Update Logo Game
**********************************************/

class CmdHandlerGameUpdateLogo : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerGameUpdateLogo();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGameUpdateLogo)

/*********************************************
 * List of Games
**********************************************/

class CmdHandlerGames : public WsjcppJsonRpc20HandlerBase {

    public:
        CmdHandlerGames();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerGames)

#endif // CMD_HADNLERS_GAMES_H
