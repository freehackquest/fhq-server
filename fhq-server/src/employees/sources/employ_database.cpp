#include <employ_server_config.h>
#include <employ_database.h>


REGISTRY_EMPLOY(EmployDatabase)

// ---------------------------------------------------------------------

EmployDatabase::EmployDatabase()
    : EmployBase(EmployDatabase::name(), {EmployServerConfig::name()}) {

}

// ---------------------------------------------------------------------

bool EmployDatabase::init(){
    // TODO
    return true;
}
