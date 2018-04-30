#ifndef CMD_HADNLERS_H
#define CMD_HADNLERS_H

#include <log.h>
#include <map>
#include <model_request.h>
#include <model_command_access.h>
#include <cmd_input_def.h>

class CmdHandlerBase {

    public:
        CmdHandlerBase(const std::string &sCmd, const std::string &sDescription);
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest) = 0;

    protected:
        std::string TAG;
        std::string m_sCmd;
        std::string m_sDescription;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

extern std::map<std::string, CmdHandlerBase*> *g_pCmdHandlers;

class CmdHandlers {
    public:
        static void initGlobalVariables();
        static void addHandler(const std::string &sName, CmdHandlerBase* pCmdHandler);
        static CmdHandlerBase *findCmdHandler(const std::string &sCmd);
};

// RegistryCmdHandler
#define REGISTRY_CMD( classname ) \
    static classname * pRegistry ## classname = new classname(); \


#endif // CMD_HADNLERS_H
