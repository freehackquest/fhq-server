#ifndef CMD_HADNLER_BASE_H
#define CMD_HADNLER_BASE_H

#include <icmdhandler.h>
#include <log.h>
#include <iserver.h>
#include <cmd_handlers.h>

class CmdHandlerBase : public ICmdHandler {

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

#endif // CMD_HADNLER_BASE_H
