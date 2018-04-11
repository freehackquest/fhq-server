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
    return true;
}

// ---------------------------------------------------------------------

