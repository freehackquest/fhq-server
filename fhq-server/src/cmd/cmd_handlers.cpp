#include <log.h>
#include <cmd_handlers.h>

// ****************************
// **** CmdHandlerBase
// ****************************

// ---------------------------------------------------------------------

CmdHandlerBase::CmdHandlerBase(const std::string &sCmd, const std::string &sDescription){
    m_sCmd = sCmd;
    m_sDescription = sDescription;
    TAG = "CmdHandlerBase(" + sCmd + ")";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // can register in global variable
    CmdHandlers::addHandler(sCmd, this);
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::cmd(){
    return m_sCmd;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::description(){
    return m_sDescription;
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerBase::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerBase::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

// ****************************
// **** CmdHandlers Map
// ****************************

std::map<std::string, CmdHandlerBase*> *g_pCmdHandlers = NULL;

// ****************************
// **** CmdHandlers
// ****************************

void CmdHandlers::initGlobalVariables(){
    if(g_pCmdHandlers == NULL){
        // Log::info(std::string(), "Create handlers map");
        g_pCmdHandlers = new std::map<std::string, CmdHandlerBase*>();
    }
}

// ---------------------------------------------------------------------

void CmdHandlers::addHandler(const std::string &sCmd, CmdHandlerBase* pCmdHandler){
    CmdHandlers::initGlobalVariables();
    if(g_pCmdHandlers->count(sCmd)){
        Log::err(sCmd, "Already registered");
    }else{
        g_pCmdHandlers->insert(std::pair<std::string, CmdHandlerBase*>(sCmd,pCmdHandler));
        // Log::info(sCmd, "Registered");
    }
}

// ---------------------------------------------------------------------

CmdHandlerBase * CmdHandlers::findCmdHandler(const std::string &sCmd){
    CmdHandlers::initGlobalVariables();
    CmdHandlerBase *pCmdHandler = NULL;

    if(g_pCmdHandlers->count(sCmd)){
        pCmdHandler = g_pCmdHandlers->at(sCmd);
    }

    if(pCmdHandler == NULL){
        Log::err(sCmd, "Not found");
    }

    return pCmdHandler;
}

// ---------------------------------------------------------------------

