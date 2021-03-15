#include "employ_chats.h"
#include <employ_database.h>
#include <employ_games.h>
#include <employ_notify.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <QString>
#include <QVariant>

REGISTRY_WJSCPP_EMPLOY(EmployChats)

// ---------------------------------------------------------------------

EmployChats::EmployChats()
    : WsjcppEmployBase(EmployChats::name(), {EmployDatabase::name(), EmployNotify::name()}) {
    TAG = EmployChats::name();
}

// ---------------------------------------------------------------------

bool EmployChats::init() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------

bool EmployChats::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------
