#include "employ_users.h"

#include <employ_database.h>
#include <wsjcpp_employees.h>
#include <employ_notify.h>

REGISTRY_WJSCPP_EMPLOY(EmployUsers)

// ---------------------------------------------------------------------

EmployUsers::EmployUsers()
    : WSJCppEmployBase(EmployUsers::name(), { EmployDatabase::name(), EmployGlobalSettings::name(), EmployNotify::name() }) {
    TAG = EmployUsers::name();
}

// ---------------------------------------------------------------------

bool EmployUsers::init() {
    WSJCppLog::info(TAG, "Start init users");
    return true;
}
