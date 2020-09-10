#ifndef CMD_HADNLERS_LXD_H
#define CMD_HADNLERS_LXD_H

#include <wsjcpp_jsonrpc20.h>
#include <model_lxd_orchestra.h>

/*********************************************
 * Any actions with the container. Actions: create, start, stop and delete container
**********************************************/

class CmdHandlerLXDContainers : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDContainers();
    virtual void handle(WsjcppJsonRpc20Request *pRequest);

private:
    std::string TAG;

    static void create_container(const std::string &name, std::string &sError, int &nErrorCode);
    static void start_container(const std::string &name, std::string &sError, int &nErrorCode);
    static void stop_container(const std::string &name, std::string &sError, int &nErrorCode);
    static void delete_container(const std::string &name, std::string &sError, int &nErrorCode);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDContainers)

/*********************************************
 * Get information about the orhestra, containers.
**********************************************/

class CmdHandlerLXDInfo : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDInfo();
    virtual void handle(WsjcppJsonRpc20Request *pRequest);
    bool get_state(const std::string& sName, std::string &sError, int &nErrorCode, nlohmann::json &jsonState);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDInfo)

/*********************************************
 * Get information about all containers.
**********************************************/

class CmdHandlerLXDList : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDList();
    virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDList)

/*********************************************
 * Execute the command in container.
**********************************************/

class CmdHandlerLXDExec : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDExec();
    void handle(WsjcppJsonRpc20Request *pRequest) override;

    static bool exec_command(const std::string &sName, const std::string &sCommand, std::string &sError,
                             int &nErrorCode, std::string &sOutput);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDExec)

/*********************************************
 * Action with files in container.
**********************************************/

class CmdHandlerLXDFile : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDFile();
    void handle(WsjcppJsonRpc20Request *pRequest) override;

    void pull_file(LXDContainer *pContainer, const std::string &sPath, std::string &sb64File,
                   std::string &sError, int &nErrorCode, bool &isDirectory);
    bool push_file(LXDContainer *pContainer, const std::string &sPath, const std::string &sb64File,
                   std::string &sError, int &nErrorCode);
    bool delete_file(const std::string &sName, const std::string &sPath, std::string &s_b64Zip);

};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDFile)

/*********************************************
 * Open container port.
**********************************************/

class CmdHandlerLXDOpenPort : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDOpenPort();
    void handle(WsjcppJsonRpc20Request *pRequest) override;
    bool is_port_valide(const std::string &sProto, const int &nPort, std::string &sError,
                        int &nErrorCode);

};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDOpenPort)

/*********************************************
 * Import container configuration from json.
**********************************************/

class CmdHandlerLXDImportService : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDImportService();
    void handle(WsjcppJsonRpc20Request *pRequest) override;
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDImportService)


/*********************************************
 * Import container configuration from zip archive.
**********************************************/

class CmdHandlerLXDImportServiceFromZip : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDImportServiceFromZip();
    void handle(WsjcppJsonRpc20Request *pRequest) override;
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDImportServiceFromZip)


/*********************************************
 * Start service.
**********************************************/

class CmdHandlerLXDStartService : public WsjcppJsonRpc20HandlerBase {

public:
    CmdHandlerLXDStartService();
    void handle(WsjcppJsonRpc20Request *pRequest) override;
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerLXDStartService)

#endif // CMD_HADNLERS_LXD_H
