#ifndef CMD_HADNLERS_LXD_H
#define CMD_HADNLERS_LXD_H

#include <cmd_handlers.h>
#include <include/model_lxd_container.h>

/*********************************************
 * Any actions with the container. Actions: create, start, stop and delete container
**********************************************/

class CmdHandlerLXDContainers : public CmdHandlerBase {

    public:
        CmdHandlerLXDContainers();
        virtual void handle(ModelRequest *pRequest);

    private:
        std::string TAG;

    static void create_container(const std::string &name, std::string &sError, int &nErrorCode);
    static void start_container(const std::string &name, std::string &sError, int &nErrorCode);
    static void stop_container(const std::string &name, std::string &sError, int &nErrorCode);
    static void delete_container(const std::string &name, std::string &sError, int &nErrorCode);
};

REGISTRY_CMD(CmdHandlerLXDContainers)

/*********************************************
 * Get information about the orhestra, containers.
**********************************************/

class CmdHandlerLXDInfo : public CmdHandlerBase {

    public:
        CmdHandlerLXDInfo();
        virtual void handle(ModelRequest *pRequest);
        bool get_state(std::string sName, std::string &sError, int &nErrorCode, nlohmann::json &jsonState);
};

REGISTRY_CMD(CmdHandlerLXDInfo)

/*********************************************
 * Get information about all containers.
**********************************************/

class CmdHandlerLXDList : public CmdHandlerBase {

    public:
        CmdHandlerLXDList();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerLXDList)

/*********************************************
 * Execute the command in container.
**********************************************/

class CmdHandlerLXDExec : public CmdHandlerBase {

public:
    CmdHandlerLXDExec();
    void handle(ModelRequest *pRequest) override;

    static bool exec_command(const std::string &sName, const std::string &sCommand, std::string &sError,
                                int &nErrorCode, std::string &sOutput);
};

REGISTRY_CMD(CmdHandlerLXDExec)

/*********************************************
 * Action with files in container.
**********************************************/

class CmdHandlerLXDFile : public CmdHandlerBase {

public:
    CmdHandlerLXDFile();
    void handle(ModelRequest *pRequest) override;

    void pull_file(LXDContainer *pContainer, const std::string &sPath, std::string &sb64File,
                             std::string &sError, int &nErrorCode, bool &isDirectory);
    bool push_file(LXDContainer *pContainer, const std::string &sPath, const std::string &sb64File,
                          std::string &sError, int &nErrorCode);
    static bool delete_file(const std::string &sName, const std::string& sPath, std::string &s_b64Zip);

};

REGISTRY_CMD(CmdHandlerLXDFile)

/*********************************************
 * Open container port.
**********************************************/

class CmdHandlerLXDOpenPort : public CmdHandlerBase {

public:
    CmdHandlerLXDOpenPort();
    void handle(ModelRequest *pRequest) override;

};

REGISTRY_CMD(CmdHandlerLXDOpenPort)

#endif // CMD_HADNLERS_LXD_H
