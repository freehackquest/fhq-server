#include <model_command_access.h>

// ---------------------------------------------------------------------

ModelCommandAccess::ModelCommandAccess(bool bAccessUnauthorized, bool bAccessUser, bool bAccessAdmin){
    m_bAccessUnauthorized = bAccessUnauthorized;
    m_bAccessUser = bAccessUser;
    m_bAccessAdmin = bAccessAdmin;
}

// ---------------------------------------------------------------------

ModelCommandAccess::ModelCommandAccess(){
    m_bAccessUnauthorized = false;
    m_bAccessUser = false;
    m_bAccessAdmin = false;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessUnauthorized(){
    return m_bAccessUnauthorized;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessUnauthorized() const {
    return m_bAccessUnauthorized;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessUser(){
    return m_bAccessUser;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessUser() const {
    return m_bAccessUser;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessAdmin(){
    return m_bAccessAdmin;
}

// ---------------------------------------------------------------------

bool ModelCommandAccess::accessAdmin() const {
    return m_bAccessAdmin;
}

// ---------------------------------------------------------------------

void ModelCommandAccess::setAccessUnauthorized(bool bNewValue){
    m_bAccessUnauthorized = bNewValue;
}

// ---------------------------------------------------------------------

void ModelCommandAccess::setAccessUser(bool bNewValue){
    m_bAccessUser = bNewValue;
}

// ---------------------------------------------------------------------

void ModelCommandAccess::setAccessAdmin(bool bNewValue){
    m_bAccessAdmin = bNewValue;
}

// ---------------------------------------------------------------------
