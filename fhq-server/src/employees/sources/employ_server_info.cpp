#include <employ_server_config.h>
#include <employ_server_info.h>


REGISTRY_EMPLOY(EmployServerInfo)

// ---------------------------------------------------------------------

EmployServerInfo::EmployServerInfo()
    : EmployBase(EmployServerInfo::name(), {"EmployDatabase", EmployServerConfig::name()}) {

}

// ---------------------------------------------------------------------

bool EmployServerInfo::init(){
    // TODO
    return false;
}
