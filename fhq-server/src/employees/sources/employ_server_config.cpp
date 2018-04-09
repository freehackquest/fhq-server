#include <employ_server_config.h>


REGISTRY_EMPLOY(EmployServerConfig)

// ---------------------------------------------------------------------

EmployServerConfig::EmployServerConfig()
    : EmployBase(EmployServerConfig::name(), {}){

}

// ---------------------------------------------------------------------

bool EmployServerConfig::init(){
    // TODO
    return true;
}
