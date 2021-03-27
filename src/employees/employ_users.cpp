#include "employ_users.h"

#include <employ_database.h>
#include <employees.h>
#include <employ_notify.h>

REGISTRY_WJSCPP_EMPLOY(EmployUsers)

// ---------------------------------------------------------------------

EmployUsers::EmployUsers()
    : WsjcppEmployBase(EmployUsers::name(), { EmployDatabase::name(), EmployGlobalSettings::name(), EmployNotify::name() }) {
    TAG = EmployUsers::name();
}

// ---------------------------------------------------------------------

bool EmployUsers::init() {
    WsjcppLog::info(TAG, "Start init users");
    return true;
}

// ---------------------------------------------------------------------

bool EmployUsers::deinit() {
    // TODO
    return true;
}