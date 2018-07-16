#include <employ_notify.h>
#include <employ_ws_server.h>
#include <employ_database.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <sys/stat.h>
#include <runtasks.h>

REGISTRY_EMPLOY(EmployNotify)

// ---------------------------------------------------------------------

EmployNotify::EmployNotify()
    : EmployBase(EmployNotify::name(), {EmployWsServer::name(), EmployDatabase::name()}) {
    TAG = EmployNotify::name();
}

// ---------------------------------------------------------------------

bool EmployNotify::init(){

    return true;
}

// ---------------------------------------------------------------------

void EmployNotify::sendNotification(ModelNotification &modelNotification){
    std::string sType = modelNotification.type();
    std::string sMessage = modelNotification.message();

    RunTasks::AddPublicEvents(QString(sType.c_str()), QString(sMessage.c_str()));

    nlohmann::json jsonMessage = modelNotification.toJson();
    jsonMessage["cmd"] = "notify";
    jsonMessage["m"] = "s0";

    // RunTasks::NotifyToAll(jsonMessage);
}
