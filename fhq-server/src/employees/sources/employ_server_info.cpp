#include <employ_server_config.h>
#include <employ_server_info.h>
#include <employ_database.h>


REGISTRY_EMPLOY(EmployServerInfo)

// ---------------------------------------------------------------------

EmployServerInfo::EmployServerInfo()
    : EmployBase(EmployServerInfo::name(), {EmployDatabase::name(), EmployServerConfig::name()}) {

}

// ---------------------------------------------------------------------

bool EmployServerInfo::init(){
    // TODO
    return true;
}
