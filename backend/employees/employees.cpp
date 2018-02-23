#include <log.h>
#include <employees.h>
#include <employ_json.h>

// ---------------------------------------------------------------------

std::map<std::string, IEmploy*> *g_pEmployees = NULL;

// ---------------------------------------------------------------------

void initEmployees(){
    if(g_pEmployees == NULL){
        Log::info("initEmployees", "Create all employees");
    }
    g_pEmployees = new std::map<std::string, IEmploy*>();

    EmployJson *pEmployJson = new EmployJson();
    std::string sEmployName = EmployJson::getEmployName();
    g_pEmployees->insert(std::pair<std::string, IEmploy*>(sEmployName,pEmployJson));
}

// ---------------------------------------------------------------------
