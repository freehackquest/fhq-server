#include "wsjcpp_employ_async_jobs.h"
#include <employ_database.h>
#include <employ_notify.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <QString>
#include <QVariant>

// ---------------------------------------------------------------------

WSJCppAsyncJob::WSJCppAsyncJob(const std::string &sName) {
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &WSJCppAsyncJob::name() {
    return m_sName;
}

// ---------------------------------------------------------------------

REGISTRY_WJSCPP_EMPLOY(WSJCppEmployAsyncJobs)

// ---------------------------------------------------------------------

WSJCppEmployAsyncJobs::WSJCppEmployAsyncJobs()
    : WsjcppEmployBase(WSJCppEmployAsyncJobs::name(), {EmployDatabase::name(), EmployNotify::name()}) {
    TAG = WSJCppEmployAsyncJobs::name();

    std::string sGroupAsyncJobs = "async_jobs";
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    pGlobalSettings->registrySetting(sGroupAsyncJobs, "jobs_fast_threads").number(4).inFile();
    pGlobalSettings->registrySetting(sGroupAsyncJobs, "jobs_slow_threads").number(1).inFile();
}

// ---------------------------------------------------------------------

bool WSJCppEmployAsyncJobs::init() {
    // TODO load from database all previous not completed tasks
    // TODO run all need threads
    return true;
}

bool WSJCppEmployAsyncJobs::deinit() {
    return true;
}

// ---------------------------------------------------------------------

void WSJCppEmployAsyncJobs::waitForDone() {

}

// ---------------------------------------------------------------------

void runTask(const std::string &sTypeName, const nlohmann::json &data);