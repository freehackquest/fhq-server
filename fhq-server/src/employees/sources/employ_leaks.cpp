#include <employ_leaks.h>
#include <employ_database.h>


REGISTRY_EMPLOY(EmployLeaks)

// ---------------------------------------------------------------------

EmployLeaks::EmployLeaks()
    : EmployBase(EmployLeaks::name(), {EmployDatabase::name()}) {
	m_vCacheLeaks.clear();
}

// ---------------------------------------------------------------------

bool EmployLeaks::init(){
	
	// TODO load leaks from files or database
    return true;
}

// ---------------------------------------------------------------------

bool EmployLeaks::addLeak(ModelLeak* pModelLeak){
	int nId = pModelLeak->id();
	if(!m_vCacheLeaks.count(pModelLeak->id())){
        m_vCacheLeaks.insert(std::pair<int, ModelLeak*>(nId,pModelLeak));
        return false;
	}
	// TODO save leak to database
	return true;
}

// ---------------------------------------------------------------------

const ModelLeak* EmployLeaks::findLeakById(int nId){
	std::map<int, ModelLeak*>::iterator it = m_vCacheLeaks.find(nId); 
	if (it != m_vCacheLeaks.end()){
		return it->second; // TODO nullpointer exception
	}
	return NULL;
}

// ---------------------------------------------------------------------

bool EmployLeaks::removeLeak(int nId){
	std::map<int, ModelLeak*>::iterator it = m_vCacheLeaks.find(nId); 
	if (it != m_vCacheLeaks.end()){
		m_vCacheLeaks.erase(it);
		return true;
	}
	// TODO remove leak from database
	return false;
}

// ---------------------------------------------------------------------

bool EmployLeaks::updateLeak(ModelLeak* pModelLeak){
	int nId = pModelLeak->id();
	std::map<int, ModelLeak*>::iterator it = m_vCacheLeaks.find(nId); 
	if (it != m_vCacheLeaks.end()){
		it->second = pModelLeak;
		return true;
	}
	// TODO save changes by leak to database
	return false;
}

// ---------------------------------------------------------------------

nlohmann::json EmployLeaks::toJson(){
	nlohmann::json jsonLeaks;
	// TODO fill json leaks
	return jsonLeaks;
}

// ---------------------------------------------------------------------
