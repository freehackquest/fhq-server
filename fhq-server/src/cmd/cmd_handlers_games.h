#ifndef CMD_HADNLERS_GAMES_H
#define CMD_HADNLERS_GAMES_H

#include <icmdhandler.h>
#include <log.h>
#include <iserver.h>
#include <cmd_handlers.h>

/*********************************************
 * Create Game
**********************************************/

class CmdGameCreateHandler : public ICmdHandler {

    public:
        CmdGameCreateHandler();
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

REGISTRY_CMD(CmdGameCreateHandler)

/*********************************************
 * Delete Game
**********************************************/

class CmdGameDeleteHandler : public ICmdHandler {

    public:
        CmdGameDeleteHandler();
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

REGISTRY_CMD(CmdGameDeleteHandler)

/*********************************************
 * Export Game
**********************************************/

class CmdGameExportHandler : public ICmdHandler {
    public:
        CmdGameExportHandler();
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

REGISTRY_CMD(CmdGameExportHandler)

/*********************************************
 * Import Game
**********************************************/

class CmdGameImportHandler : public ICmdHandler {

    public:
        CmdGameImportHandler();
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

REGISTRY_CMD(CmdGameImportHandler)


/*********************************************
 * Info Game
**********************************************/

class CmdGameInfoHandler : public ICmdHandler {

    public:
        CmdGameInfoHandler();
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

REGISTRY_CMD(CmdGameInfoHandler)

/*********************************************
 * Update Game
**********************************************/

class CmdGameUpdateHandler : public ICmdHandler {

    public:
        CmdGameUpdateHandler();
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

REGISTRY_CMD(CmdGameUpdateHandler)


/*********************************************
 * Update Logo Game
**********************************************/

class CmdGameUpdateLogoHandler : public ICmdHandler {

    public:
        CmdGameUpdateLogoHandler();
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

REGISTRY_CMD(CmdGameUpdateLogoHandler)

/*********************************************
 * List of Games
**********************************************/

class CmdGamesHandler : public ICmdHandler {

    public:
        CmdGamesHandler();
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

REGISTRY_CMD(CmdGamesHandler)

#endif // CMD_HADNLERS_GAMES_H
