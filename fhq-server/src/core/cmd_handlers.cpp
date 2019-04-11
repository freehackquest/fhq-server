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

    m_bAccessUnauthorized = false;
    m_bAccessUser = false;
    m_bAccessAdmin = false;

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

bool CmdHandlerBase::accessUnauthorized() {
    return m_bAccessUnauthorized;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::accessUser() {
    return m_bAccessUser;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::accessAdmin() {
    return m_bAccessAdmin;
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
    m_bAccessUnauthorized = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessUser(bool bAccess){
    m_bAccessUser = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessAdmin(bool bAccess){
    m_bAccessAdmin = bAccess;
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

CmdInputDef &CmdHandlerBase::addInputDef(const std::string &name) {
    // TODO check duplicates
    m_vInputs.push_back(CmdInputDef(name));
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::addInputDef_require_string(const std::string &sName, const std::string &sDescription) {
    CmdInputDef pStringDef(sName);
    pStringDef.string_().description(sDescription).required();
    m_vInputs.push_back(pStringDef);
    return m_vInputs[m_vInputs.size()-1];
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

