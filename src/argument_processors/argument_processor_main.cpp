
#include "argument_processor_main.h"

#include <wsjcpp_core.h>
#include <employees.h>

#include "argument_processor_api.h"
#include "argument_processor_database.h"
#include "argument_processor_config.h"

// ---------------------------------------------------------------------
// ArgumentProcessorMain

ArgumentProcessorMain::ArgumentProcessorMain() 
: WsjcppArgumentProcessor({"main"}, "FreeHackQuest Server", "FreeHackQuest Server") {
    TAG = "ArgumentProcessorMain";
    // registrySingleArgument("--single", "What exactly do this single param?");
    registryParameterArgument("--workdir", "<path>", "workdir");
    registryParameterArgument("-wd", "<path>", "workdir (short)");

    // registryExample("here example of command");
    registryProcessor(new ArgumentProcessorVersion());
    registryProcessor(new ArgumentProcessorDatabase());
    registryProcessor(new ArgumentProcessorConfig());
    registryProcessor(new ArgumentProcessorApi());

    m_sWorkDir = "";
    std::vector<std::string> vDefaultsWorkdirs = {
        WsjcppCore::getCurrentDirectory() + "./data/",
        WsjcppCore::getCurrentDirectory() + "./ci/travis/data/",
        "/usr/share/fhq-server/data"
    };
    for (int i = 0; i < vDefaultsWorkdirs.size(); i++) {
        if (WsjcppCore::dirExists(vDefaultsWorkdirs[i])) {
            m_sWorkDir = vDefaultsWorkdirs[i];
        }
    }
}

bool ArgumentProcessorMain::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

bool ArgumentProcessorMain::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    if (sArgumentName == "--workdir" || sArgumentName == "-wd") {
        setWorkDir(sValue);
        return true;
    }
    return false;
}

int ArgumentProcessorMain::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    return -10;
}

bool ArgumentProcessorMain::setWorkDir(const std::string &sWorkDir_) {
    auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

    std::string sWorkDir = WsjcppCore::getCurrentDirectory() + sWorkDir_;
    sWorkDir = WsjcppCore::doNormalizePath(sWorkDir);

    std::cout << "\n Workdir: " << sWorkDir << " \n\n";
    if (!WsjcppCore::dirExists(sWorkDir)) {
        WsjcppLog::err(TAG, "Directory '" + sWorkDir + "' did'not exists");
        return -1;
    }

    // configure employ
    if (sWorkDir != "") {
        pGlobalSettings->setWorkDir(sWorkDir);
        pGlobalSettings->update("work_dir", sWorkDir);
    }

    std::string sDirLogs = WsjcppCore::doNormalizePath(sWorkDir + "/logs");
    if (!WsjcppCore::dirExists(sDirLogs)) {
        WsjcppCore::makeDir(sDirLogs);
    }
    pGlobalSettings->update("log_dir", sDirLogs);
    WsjcppLog::setLogDirectory(sDirLogs);
    m_sWorkDir = sWorkDir;
    return true;
}

const std::string &ArgumentProcessorMain::getWorkDir() {
    return m_sWorkDir;
}

// ---------------------------------------------------------------------
// ArgumentProcessorVersion

ArgumentProcessorVersion::ArgumentProcessorVersion() 
: WsjcppArgumentProcessor({"version", "ver", "--version"}, "Print version", "Print version") {
    TAG = "ArgumentProcessorVersion";
}

// ---------------------------------------------------------------------

int ArgumentProcessorVersion::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    std::cout << WSJCPP_APP_NAME << "-" << WSJCPP_APP_VERSION << "\n";
    return 0;
}
