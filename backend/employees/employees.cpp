#include <log.h>
#include <employees.h>
#include <employ_json.h>
#include <employ_settings.h>

// ---------------------------------------------------------------------

std::map<std::string, IEmploy*> *g_pEmployees = NULL;

// ---------------------------------------------------------------------

void initEmployees(){
    if(g_pEmployees != NULL){
        return;
    }

    Log::info("initEmployees", "Create all employees");
    g_pEmployees = new std::map<std::string, IEmploy*>();

    std::string sEmployName = "";

    EmployJson *pEmployJson = new EmployJson();
    sEmployName = EmployJson::getEmployName();
    g_pEmployees->insert(std::pair<std::string, IEmploy*>(sEmployName,pEmployJson));

    EmploySettings *pEmploySettings = new EmploySettings();
    sEmployName = EmploySettings::getEmployName();
    g_pEmployees->insert(std::pair<std::string, IEmploy*>(sEmployName,pEmploySettings));
    Log::info("initEmployees", "Employees created");
}

// ---------------------------------------------------------------------
