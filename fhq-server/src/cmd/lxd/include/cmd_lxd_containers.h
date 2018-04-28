#ifndef CMD_LXD_CONTAINERS
#define CMD_LXD_CONTAINERS

#include <iserver.h>
#include <cmd_handlers.h>

class CmdLXDContainersHandler : public ICmdHandler {

    public:
        CmdLXDContainersHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        std::string TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;

        void create_container(std::string name, QJsonObject &jsonResponse);
        void start_container(std::string name, QJsonObject &jsonResponse);
        void stop_container(std::string name, QJsonObject &jsonResponse);
        void delete_container(std::string name, QJsonObject &jsonResponse);
};

REGISTRY_CMD(CmdLXDContainersHandler)

#endif // CMD_LXD_CONTAINERS

