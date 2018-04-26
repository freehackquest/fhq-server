#include <employ_ws_server.h>
#include <employ_server_config.h>
#include <employ_server_info.h>

#include <QSqlQuery>
#include <QSqlRecord>


REGISTRY_EMPLOY(EmployWsServer)

// ---------------------------------------------------------------------

EmployWsServer::EmployWsServer()
    : EmployBase(EmployWsServer::name(), {"start_ws_server", EmployServerConfig::name(), EmployServerInfo::name()}) {
	TAG = EmployWsServer::name();
}

// ---------------------------------------------------------------------

bool EmployWsServer::init(){

    return true;
}

// ---------------------------------------------------------------------
