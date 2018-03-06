#include <model_command_access.h>

// ---------------------------------------------------------------------

ModelCommandAccess::ModelCommandAccess(bool bAccessUnauthorized, bool bAccessUser, bool bAccessTester, bool bAccessAdmin){
    m_bAccessUnauthorized = bAccessUnauthorized;
    m_bAccessUser = bAccessUser;
    m_bAccessTester = bAccessTester;
    m_bAccessAdmin = bAccessAdmin;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessUnauthorized(){
    return m_bAccessUnauthorized;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessUser(){
    return m_bAccessUser;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessTester(){
    return m_bAccessTester;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessAdmin(){
    return m_bAccessAdmin;
}

// ---------------------------------------------------------------------
