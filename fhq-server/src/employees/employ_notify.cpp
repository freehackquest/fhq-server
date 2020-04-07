
#include <employees.h>
#include <employ_notify.h>
#include <employ_database.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <sys/stat.h>
#include <runtasks.h>

REGISTRY_WJSCPP_EMPLOY(EmployNotify)

// ---------------------------------------------------------------------

std::string EmployNotify::GAMES = "games";
std::string EmployNotify::QUESTS = "quests";
std::string EmployNotify::USERS = "users";
std::string EmployNotify::SCOREBOARD = "scoreboard";
std::string EmployNotify::LEAKS = "leaks";

// ---------------------------------------------------------------------

EmployNotify::EmployNotify()
    : WsjcppEmployBase(EmployNotify::name(), { EmployServer::name(), EmployDatabase::name() }) {
    TAG = EmployNotify::name();
}

// ---------------------------------------------------------------------

bool EmployNotify::init() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

bool EmployNotify::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------

void EmployNotify::sendNotification(ModelNotification &modelNotification) {
    std::string sType = modelNotification.type();
    std::string sMessage = modelNotification.message();

    RunTasks::AddPublicEvents(sType, sMessage);

    nlohmann::json jsonMessage = modelNotification.toJson();
    jsonMessage["cmd"] = "notify";
    jsonMessage["m"] = "s0";

    RunTasks::NotifyToAll(jsonMessage);
}

// ---------------------------------------------------------------------

void EmployNotify::notifyWarning(const std::string &sSection, const std::string &sMessage) {
    sendNotification("warning", sSection, sMessage);
}

// ---------------------------------------------------------------------

void EmployNotify::notifyDanger(const std::string &sSection, const std::string &sMessage) {
    sendNotification("danger", sSection, sMessage);
}

// ---------------------------------------------------------------------

void EmployNotify::notifyInfo(const std::string &sSection, const std::string &sMessage) {
    sendNotification("info", sSection, sMessage);
}

// ---------------------------------------------------------------------

void EmployNotify::notifySuccess(const std::string &sSection, const std::string &sMessage) {
    sendNotification("success", sSection, sMessage);
}

// ---------------------------------------------------------------------


void EmployNotify::sendNotification(const std::string &sType,
                                    const std::string &sSection,
                                    const std::string &sMessage) {

    RunTasks::AddPublicEvents(sType, sMessage);

    nlohmann::json jsonMessage;
    jsonMessage["type"] = sType;
    jsonMessage["section"] = sSection;
    jsonMessage["message"] = sMessage;
    jsonMessage["cmd"] = "notify";
    jsonMessage["m"] = "s0";

    RunTasks::NotifyToAll(jsonMessage);
}

// ---------------------------------------------------------------------
