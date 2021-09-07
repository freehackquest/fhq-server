
#include "argument_processor_config.h"
#include <wsjcpp_core.h>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <employees.h>
#include <runtasks.h>
#include <unistd.h>  // getpass
#include <utils_lxd.h>

// ---------------------------------------------------------------------
// ArgumentProcessorConfig

ArgumentProcessorConfig::ArgumentProcessorConfig() 
: WsjcppArgumentProcessor({"config", "cfg"}, "Configs", "All what depened to config processing") {
    TAG = "ArgumentProcessorConfig";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    registryProcessor(new ArgumentProcessorConfigTest());
    registryProcessor(new ArgumentProcessorConfigPrint());
    registryProcessor(new ArgumentProcessorConfigTestEmail());
    registryProcessor(new ArgumentProcessorConfigLxdEnable());
    registryProcessor(new ArgumentProcessorConfigLxdDisable());
    registryProcessor(new ArgumentProcessorConfigLxdConfigure());
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

// ---------------------------------------------------------------------
// ArgumentProcessorConfigPrint

ArgumentProcessorConfigPrint::ArgumentProcessorConfigPrint() 
: WsjcppArgumentProcessor({"print"}, "Print config", "Print config") {
    TAG = "ArgumentProcessorConfigPrint";
}

int ArgumentProcessorConfigPrint::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    WsjcppEmployees::init({});
    std::cout << "\n * Show settings\n\n";
    pGlobalSettings->printSettings();
    std::cout << "\n * Done\n\n";
    return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorConfigTestEmail

ArgumentProcessorConfigTestEmail::ArgumentProcessorConfigTestEmail() 
: WsjcppArgumentProcessor({"test-email"}, "Send test e-mail", "Send test e-mail") {
    TAG = "ArgumentProcessorConfigTestEmail";
}

int ArgumentProcessorConfigTestEmail::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    WsjcppEmployees::init({});
    std::cout << "\n * Send test mail\n\n";
    std::string sTo = pGlobalSettings->get("mail_system_message_admin_email").getStringValue();
    std::string sSubject = "Test Mail";
    std::string sContent = "Welcome to Free Hack Quest!\r\n\r\nHow are you?";
    RunTasks::MailSend(sTo, sSubject, sContent);
    RunTasks::waitForDone();
    return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorConfigLxdEnable

ArgumentProcessorConfigLxdEnable::ArgumentProcessorConfigLxdEnable() 
: WsjcppArgumentProcessor({"lxd-enable"}, "Enable lxd mode", "Enable lxd mode") {
    TAG = "ArgumentProcessorConfigLxdEnable";
}

int ArgumentProcessorConfigLxdEnable::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    WsjcppEmployees::init({});
    pGlobalSettings->update("lxd_mode", true);
    std::cout << "\nCurrent LXD mode: " << pGlobalSettings->get("lxd_mode").convertValueToString(false) << "\n";
    return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorConfigLxdDisable

ArgumentProcessorConfigLxdDisable::ArgumentProcessorConfigLxdDisable() 
: WsjcppArgumentProcessor({"lxd-enable"}, "Disable lxd mode", "Disable lxd mode") {
    TAG = "ArgumentProcessorConfigLxdDisable";
}

int ArgumentProcessorConfigLxdDisable::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    WsjcppEmployees::init({});
    pGlobalSettings->update("lxd_mode", false);
    std::cout << "\nCurrent LXD mode: " << pGlobalSettings->get("lxd_mode").convertValueToString(false) << "\n";
    return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorConfigLxdConfigure

ArgumentProcessorConfigLxdConfigure::ArgumentProcessorConfigLxdConfigure() 
: WsjcppArgumentProcessor({"lxd-configure"}, "manual configure lxd", "Manual configure HTTPS connection with LXD. \n You need generated SSL cert and key in /etc/fhq-server/lxd") {
    TAG = "ArgumentProcessorConfigLxdConfigure";
}

int ArgumentProcessorConfigLxdConfigure::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    // auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    // config lxd
    std::string sError;
    WsjcppEmployees::init({});
    if (UtilsLXDAuth::check_trust_certs(sError)) {
        std::cout << "\nGOOD HTTPS connection with LXD\n\n";
    } else if (!sError.empty()) {
        WsjcppLog::err(TAG, "\nBAD HTTPS connection with LXD\n\n: " + sError);
        return -1;
    } else {
        char *pPassword = getpass("\nPlease enter your password for LXD:");
        std::string sPass(pPassword);
        if (UtilsLXDAuth::connect_with_lxd(sPass, sError))
            std::cout << "\nWell, you added your certificates to LXD.\n\n";
        else {
            std::cout << "\nBad, I can not make certificates trusted. Maybe the password was wrong? \nError:" + sError + "\n\n";
            return -1;
        }
    }
    return 0;
}