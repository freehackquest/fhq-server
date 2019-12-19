#include <employ_users.h>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_notify.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <sys/stat.h>


REGISTRY_EMPLOY(EmployUsers)

// ---------------------------------------------------------------------

EmployUsers::EmployUsers()
    : EmployBase(EmployUsers::name(), {EmployDatabase::name(), EmploySettings::name(), EmployNotify::name()}) {
    TAG = EmployUsers::name();
}

// ---------------------------------------------------------------------

bool EmployUsers::init(){
    Log::info(TAG, "Start init users");
    return true;
}
