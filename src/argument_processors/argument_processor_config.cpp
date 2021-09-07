
#include "argument_processor_config.h"
#include <wsjcpp_core.h>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <employees.h>

// ---------------------------------------------------------------------
// ArgumentProcessorConfig

ArgumentProcessorConfig::ArgumentProcessorConfig() 
: WsjcppArgumentProcessor({"config", "cfg"}, "Configs", "All what depened to config processing") {
    TAG = "ArgumentProcessorConfig";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    registryProcessor(new ArgumentProcessorConfigTest());
}

int ArgumentProcessorConfig::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");
    return -10;
}

// ---------------------------------------------------------------------
// ArgumentProcessorConfigTest

ArgumentProcessorConfigTest::ArgumentProcessorConfigTest() 
: WsjcppArgumentProcessor({"test"}, "Test config", "Test config") {
    TAG = "ArgumentProcessorConfigTest";
}

int ArgumentProcessorConfigTest::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    std::cout << "\n * Check Server Config\n\n";
    if (!pGlobalSettings->init()) {
        std::cout << "\n * FAIL\n\n";
        return -1;
    }
    std::cout << "\n * Success\n\n";
    return 0;
}