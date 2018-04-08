#include <log.h>
#include <employees.h>
#include <employ_server_info.h>
#include <employ_settings.h>

// ---------------------------------------------------------------------

std::map<std::string, EmployBase*> *g_pEmployees = NULL;

// ---------------------------------------------------------------------

void Employees::initGlobalVariables(){
    if(g_pEmployees == NULL){
        Log::info(std::string(), "Init employees map");
        g_pEmployees = new std::map<std::string, EmployBase*>();
    }
}

// ---------------------------------------------------------------------

void Employees::addEmploy(const std::string &sName, EmployBase* pEmploy){
    Employees::initGlobalVariables();
    if(g_pEmployees->count(sName)){
        Log::err(sName, "Already registered");
    }else{
        g_pEmployees->insert(std::pair<std::string, EmployBase*>(sName,pEmploy));
        Log::info(sName, "Registered");
    }
}

// ---------------------------------------------------------------------

// EmployBase
EmployBase::EmployBase(const std::string &sName, const std::vector<std::string> &vAfter){
    TAG = sName;
    m_sName = sName;

    for(unsigned int i = 0; i < vAfter.size(); i++){
        m_vLoadAfter.push_back(vAfter[i]);
    }
    Employees::addEmploy(m_sName, this);
}

// ---------------------------------------------------------------------

