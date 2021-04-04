// Copyright (c) 2015-2020 FreeHackQuest <freehackquest@gmail.com>

#ifndef CMD_HADNLERS_GAMES_H
#define CMD_HADNLERS_GAMES_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerGameCreate : public CmdHandlerBase {

    public:
        CmdHandlerGameCreate();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerGameDelete : public CmdHandlerBase {

    public:
        CmdHandlerGameDelete();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerGameExport : public CmdHandlerBase {
    public:
        CmdHandlerGameExport();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerGameImport : public CmdHandlerBase {

    public:
        CmdHandlerGameImport();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// depreacted 

class CmdHandlerGameInfo : public CmdHandlerBase {

    public:
        CmdHandlerGameInfo();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerGamesInfo : public CmdHandlerBase {

    public:
        CmdHandlerGamesInfo();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerGameUpdate : public CmdHandlerBase {

    public:
        CmdHandlerGameUpdate();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerGameUpdateLogo : public CmdHandlerBase {

    public:
        CmdHandlerGameUpdateLogo();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerGames : public CmdHandlerBase {

    public:
        CmdHandlerGames();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

#endif // CMD_HADNLERS_GAMES_H
