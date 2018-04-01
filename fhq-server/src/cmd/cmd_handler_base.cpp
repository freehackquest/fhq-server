#include <log.h>
#include <cmd_handler_base.h>

// ---------------------------------------------------------------------

CmdHandlerBase::CmdHandlerBase(const std::string &sCmd, const std::string &sDescription){
    m_sCmd = sCmd;
    m_sDescription = sDescription;
    TAG = "CmdHandlerBase(" + sCmd + ")";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // can register in global variable
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



