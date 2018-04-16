#include <log.h>
#include <employees.h>
#include <employ_server_info.h>
#include <employ_settings.h>
#include <algorithm>

// ---------------------------------------------------------------------

std::map<std::string, EmployBase*> *g_pEmployees = NULL;
std::vector<std::string> *g_pInitEmployees = NULL;

// ---------------------------------------------------------------------

void Employees::initGlobalVariables(){
    if(g_pEmployees == NULL){
        // Log::info(std::string(), "Create employees map");
        g_pEmployees = new std::map<std::string, EmployBase*>();
    }
    if(g_pInitEmployees == NULL){
		// Log::info(std::string(), "Create init employees vector");
		g_pInitEmployees = new std::vector<std::string>();
	}
}

// ---------------------------------------------------------------------

void Employees::addEmploy(const std::string &sName, EmployBase* pEmploy){
    Employees::initGlobalVariables();
    if(g_pEmployees->count(sName)){
        Log::err(sName, "Already registered");
    }else{
        g_pEmployees->insert(std::pair<std::string, EmployBase*>(sName,pEmploy));
        // Log::info(sName, "Registered");
    }
}

// ---------------------------------------------------------------------

bool Employees::init(const std::vector<std::string> &vStart){
	Employees::initGlobalVariables();

    for(unsigned int i = 0; i < vStart.size(); i++){
        g_pInitEmployees->push_back(vStart[i]);
    }

	std::string TAG = "Employees_init";
	bool bRepeat = true;
	while(bRepeat){
		bRepeat = false;
		std::map<std::string, EmployBase*>::iterator it = g_pEmployees->begin();
		for (; it!=g_pEmployees->end(); ++it){
			std::string sEmployName = it->first;
			EmployBase *pEmploy = it->second;

			if(std::find(g_pInitEmployees->begin(), g_pInitEmployees->end(), sEmployName) != g_pInitEmployees->end()) {
				continue;
			}

			unsigned int nRequireLoaded = 0;
			for(unsigned int i = 0; i < pEmploy->loadAfter().size(); i++){
				std::string sRequireEmploy = pEmploy->loadAfter()[i];
				if(std::find(g_pInitEmployees->begin(), g_pInitEmployees->end(), sRequireEmploy) != g_pInitEmployees->end()) {
					nRequireLoaded++;
				}
			}
			if(pEmploy->loadAfter().size() == nRequireLoaded){
				if(!pEmploy->init()){
					Log::err(TAG, "Init " + sEmployName + " ... FAIL");
					return false;
				}
				g_pInitEmployees->push_back(sEmployName);
				bRepeat = true;
				Log::info(TAG, "Init " + sEmployName + " ... OK");
			}
		}
	}
	return true;
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

const std::vector<std::string> &EmployBase::loadAfter(){
	return m_vLoadAfter;
}
