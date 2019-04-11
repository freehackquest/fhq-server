#ifndef CMD_HADNLERS_H
#define CMD_HADNLERS_H

#include <map>
#include <utils_logger.h>
#include <model_request.h>
#include <cmd_input_def.h>

class CmdHandlerBase {

    public:
        CmdHandlerBase(const std::string &sCmd, const std::string &sDescription);
        virtual std::string cmd();
        virtual std::string description();
        std::string activatedFromVersion();
        std::string deprecatedFromVersion();
        bool accessUnauthorized();
        bool accessUser();
        bool accessAdmin();

        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest) = 0;

        // virtual void success(nlohmann::json jsonResponse);
        // virtual void error(int nCode, const std::string &sErrorMessage);
        
    protected:
        void setAccessUnauthorized(bool bAccess);
        void setAccessUser(bool bAccess);
        void setAccessAdmin(bool bAccess);
        void setActivatedFromVersion(const std::string &sActivatedFromVersion);
        void setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion);
        CmdInputDef &addInputDef(const std::string &name);
        CmdInputDef &addInputDef_require_string(const std::string &sName, const std::string &sDescription);

        std::string TAG;
        std::string m_sCmd;
        std::string m_sDescription;

    private:
        std::vector<CmdInputDef> m_vInputs; // TODO redesign to map
        // std::map<std::string, CmdInputDef*> *m_vCmdInputDefs;
        std::string m_sActivatedFromVersion;
        std::string m_sDeprecatedFromVersion;
        bool m_bAccessUnauthorized;
        bool m_bAccessUser;
        bool m_bAccessAdmin;
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
