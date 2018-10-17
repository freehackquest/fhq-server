#include <utils_logger.h>
#include <cmd_handlers.h>
#include <employ_ws_server.h>

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

std::string CmdHandlerBase::activatedFromVersion(){
    return m_sActivatedFromVersion;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::deprecatedFromVersion(){
    return m_sDeprecatedFromVersion;
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

void CmdHandlerBase::setAccessUnauthorized(bool bAccess){
    m_modelCommandAccess.setAccessUnauthorized(bAccess);
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessUser(bool bAccess){
    m_modelCommandAccess.setAccessUser(bAccess);
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessAdmin(bool bAccess){
    m_modelCommandAccess.setAccessAdmin(bAccess);
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerBase::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setActivatedFromVersion(const std::string &sActivatedFromVersion){
    m_sActivatedFromVersion = sActivatedFromVersion;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion){
    m_sDeprecatedFromVersion = sDeprecatedFromVersion;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerBase::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

/*
void CmdHandlerBase::success(nlohmann::json jsonResponse){
    EmployWsServer *pEmployWsServer = findEmploy<EmployWsServer>();
    // TODO sendMessageSuccess
    // and remove from ModelRequests
}

// ---------------------------------------------------------------------

void CmdHandlerBase::error(int nCode, const std::string &sErrorMessage){
    EmployWsServer *pEmployWsServer = findEmploy<EmployWsServer>();
    // TODO sendMessageError
    // and remove from ModelRequests

}
// ---------------------------------------------------------------------
*/

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

