#include <iostream>
#include <string>
#include <unistd.h> // getpass

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <signal.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/time.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <syslog.h>
#include <QtCore>
#include <QFile>
#include <QString>
#include <utils_logger.h>
#include <websocketserver.h>
#include <utils_prepare_deb_package.h>
#include <utils_lxd.h>
#include <employees.h>
#include <employ_server_config.h>
#include <employ_server_info.h>
#include <employ_database.h>
#include <employ_settings.h>
#include <employ_images.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <export_list_of_handlers.h>
#include <export_struct_of_storage.h>
#include <export_libfhqcli_web_js.h>
#include <export_libfhqcli_py.h>
#include <runtasks.h>
#include <light_http_server.h>
#include <http_handler_web_admin_folder.h>
#include <jobs_pool.h>
#include <fallen.h>

LightHttpServer g_httpServer;

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);
    std::string TAG = "MAIN";
    Log::setdir("/var/log/fhq-server");

    FallenHelpParseArgs helpArgs(argc, argv);
    helpArgs.setAppName(FHQSRV_APP_NAME);
    helpArgs.setAppVersion(FHQSRV_VERSION);

    helpArgs.addHelp("help", "-h", FallenHelpParseArgType::SINGLE_OPTION, "This help");
    helpArgs.addHelp("version", "-v", FallenHelpParseArgType::SINGLE_OPTION, "Print version");
    helpArgs.addHelp("show-handlers", "-sh", FallenHelpParseArgType::SINGLE_OPTION, "Show handlers");
    helpArgs.addHelp("show-storage-struct", "-sh-ss", FallenHelpParseArgType::SINGLE_OPTION, "Show Storage Struct");
    helpArgs.addHelp("export-libfhqcli-py", "-exlp", FallenHelpParseArgType::SINGLE_OPTION, "Export libfhqcli-py (python)");
    helpArgs.addHelp("export-libfhqcli-web-javascript", "-exlwjs", FallenHelpParseArgType::SINGLE_OPTION, "Export libfhqcli-web-js (javascript)");
    helpArgs.addHelp("show-employees", "-se", FallenHelpParseArgType::SINGLE_OPTION, "Show employees");
    helpArgs.addHelp("show-settings", "-ss", FallenHelpParseArgType::SINGLE_OPTION, "Show settings");
    helpArgs.addHelp("set-setting", "-set", FallenHelpParseArgType::PARAMETER, "Set setting value like 'mail_username=some@where.org'");
    helpArgs.addHelp("send-test-mail", "-stm", FallenHelpParseArgType::SINGLE_OPTION, "Send test mail");
    helpArgs.addHelp("prepare-deb", "-pd", FallenHelpParseArgType::SINGLE_OPTION, "Prepare Deb Package");
    helpArgs.addHelp("check-server-config", "-csc", FallenHelpParseArgType::SINGLE_OPTION, "Check server config");
    helpArgs.addHelp("make-config-linux", "-mcl", FallenHelpParseArgType::SINGLE_OPTION, "Create config file for Linux: /etc/fhq-server/fhq-server.conf");
    helpArgs.addHelp("check-database-connection", "-cdc", FallenHelpParseArgType::SINGLE_OPTION, "Check database conenction");
    helpArgs.addHelp("manual-create-database", "-mcd", FallenHelpParseArgType::SINGLE_OPTION, "Manual create database");
    helpArgs.addHelp("manual-configure-lxd", "-mclxd", FallenHelpParseArgType::SINGLE_OPTION, "Manual configure HTTPS connection with LXD. \n You need generated SSL cert and key in /etc/fhq-server/lxd");
    helpArgs.addHelp("lxd-enable", "-uplxd", FallenHelpParseArgType::SINGLE_OPTION, "Enable lxd mode");
    helpArgs.addHelp("lxd-disable", "-downlxd", FallenHelpParseArgType::SINGLE_OPTION, "Disable lxd mode");
    helpArgs.addHelp("start", "-s", FallenHelpParseArgType::SINGLE_OPTION, "Start server");
    helpArgs.addHelp("--workdir", "-wd", FallenHelpParseArgType::PARAMETER, "Set work dir (logs, configs aand etc...)");

    std::string sWorkDir = "";
    if (helpArgs.has("--workdir")) {
        sWorkDir = helpArgs.option("--workdir");
        std::cout << "\n Workdir " << sWorkDir << " \n\n";
        if (!Fallen::dirExists(sWorkDir)) {
            Log::err(TAG, "Directory '" + sWorkDir + "' did'not exists");
            return -1;
        }
        std::string sDirLogs = sWorkDir + "/logs";
        if (!Fallen::dirExists(sDirLogs)) {
            Fallen::makeDir(sDirLogs);
        }
        Log::setdir(sDirLogs);
    }

    if (helpArgs.has("help")) {
        helpArgs.printHelp();
        return 0;
    } else if (helpArgs.has("show-handlers")) {
        ExportListOfHandlers::print();
        return 0;
    } else if (helpArgs.has("show-storage-struct")) {
        ExportStructOfStorage::print();
        return 0;
    } else if (helpArgs.has("export-libfhqcli-py")) {
        ExportLibFHQCliPy::exportLib();
        return 0;
    } else if (helpArgs.has("export-libfhqcli-web-javascript")) {
        ExportLibFHQCliWebJS::exportLib();
        return 0;
    } else if (helpArgs.has("show-employees")) {
        std::cout << " * Employees (" << g_pEmployees->size() << "):\n";
        std::map<std::string, EmployBase*>::iterator it = g_pEmployees->begin();
        for (; it!=g_pEmployees->end(); ++it){
            std::string sEmployName = it->first;
            EmployBase* pEmployBase = it->second;
            std::cout << " |--- * " << sEmployName << "\n";
            if(pEmployBase->loadAfter().size() > 0){
                for(int i = 0; i < pEmployBase->loadAfter().size(); i++){
                    std::cout << " |    +--- * after: " << pEmployBase->loadAfter().at(i) << "\n";
                }
            }
            std::cout << " |  \n";
        }
        return 0;
    } else if (helpArgs.has("version")) {
        std::cout << FHQSRV_APP_NAME << "-" << FHQSRV_VERSION << "\n";
        return 0;
    } else if (helpArgs.has("prepare-deb")) {
        UtilsPrepareDebPackage::prepare("","tmpdeb");
        return 0;
    } else if (helpArgs.has("check-server-config")) {
        std::cout << "\n * Check Server Config\n\n";
        EmployServerConfig *pConfig = new EmployServerConfig();
        if (!pConfig->init()) {
            std::cout << "\n * FAIL\n\n";
        } else {
            std::cout << "\n * Success\n\n";
        }
        return 0;
    } else if (helpArgs.has("check-database-connection")) {
        std::cout << "\n * Check Database Connection\n\n";
        if (!Employees::init({})) {
            Log::err(TAG, "Could not init database module");
            return -1;
        }
        EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
        QSqlDatabase *db = pDatabase->database();
        if (!db->open()){
            Log::err(TAG, "Could not connect to database, please check config");
            return -1;
        }
        std::cout << "\n * Success\n\n";
        return 0;
    } else if (helpArgs.has("show-settings")) {
        Employees::init({});
        EmploySettings *pSettings = findEmploy<EmploySettings>();
        std::cout << "\n * Show settings\n\n";
        pSettings->printSettings();
        std::cout << "\n * Done\n\n";
        return 0;
    } else if (helpArgs.has("set-setting")) {
        Employees::init({});
        EmploySettings *pSettings = findEmploy<EmploySettings>();
        std::string sSetting = helpArgs.option("set-setting");
        std::cout << "\n Try set setting " << sSetting << " \n\n";
        std::string sSettName = "";
        std::istringstream f(sSetting);
        getline(f, sSettName, '=');
        if (sSettName.length() == sSetting.length()) {
            Log::err(TAG, "Could not split by '=' for a '" + sSetting + "'");
            return -1;
        }
        std::string sSettValue = sSetting.substr(sSettName.length()+1);
        if (!pSettings->hasSett(sSettName)) {
            Log::err(TAG, "Not support settings with name '" + sSettName + "'");
            return -1;
        }
        std::string sSettType = pSettings->getSettType(sSettName);
        if (sSettType == "string" || sSettType == "password") {
            pSettings->setSettString(sSettName, QString::fromStdString(sSettValue));
        } else if (sSettType == "boolean") {
            if (sSettValue != "true" && sSettValue != "yes" && sSettValue != "false" && sSettValue != "no") {
                Log::err(TAG, "Expected value boolean (true|yes|false|no), but got '" + sSettValue + "' for '" + sSettName + "'");
                return -1;
            }
            pSettings->setSettBoolean(sSettName, sSettValue == "true" || sSettValue == "yes");
        } else if (sSettType == "integer") {
            int nSettValue = std::stoi(sSettValue);
            pSettings->setSettInteger(sSettName, nSettValue);
        } else {
            Log::err(TAG, "Not support settings datatype with name '" + sSettName + "'");
            return -1;
        }
        return 0;
    } else if (helpArgs.has("send-test-mail")) {
        Employees::init({});
        EmploySettings *pSettings = findEmploy<EmploySettings>();
        std::cout << "\n * Send test mail\n\n";
        std::string sTo = pSettings->getSettString("mail_system_message_admin_email").toStdString();
        std::string sSubject = "Test Mail";
        std::string sContent = "Welcome to Free Hack Quest!\r\n\r\nHow are you?";
        RunTasks::MailSend(sTo, sSubject, sContent);
        RunTasks::waitForDone();
        return 0;
    } else if (helpArgs.has("manual-create-database")) {
        std::cout << "\n * Manual create database\n\n";
        EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
        if(!pServerConfig->init()){
            std::cout << "\n * Failed on init server config\n\n";
            return -1;
        }
        EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

        // enter mysql root password
        char *pPassword=getpass("Enter MySQL root password: ");
        std::string sRootPassword(pPassword);
        std::string sError;
        if(!pDatabase->manualCreateDatabase(sRootPassword, sError)){
            std::cout << "\n * Failed: " << sError << "\n\n";
            return -1;
        }

        // init database
        if(!pDatabase->init()){
            std::cout << "\n * Failed on init database structure\n\n";
            return -1;
        }

        std::cout << "\n * Done\n\n";
        return 0;
    } else if (helpArgs.has("manual-configure-lxd")) {
        std::string sError;
        Employees::init({});
        if (UtilsLXDAuth::check_trust_certs(sError))
            std::cout << "\nGOOD HTTPS connection with LXD\n\n";
        else if (!sError.empty()){
            Log::err(TAG, "\nBAD HTTPS connection with LXD\n\n: " + sError);
            return -1;
        }
        else {
            char *pPassword=getpass("\nPlease enter your password for LXD:");
            std::string sPass(pPassword);
            if (UtilsLXDAuth::connect_with_lxd(sPass, sError))
                std::cout << "\nWell, you added your certificates to LXD.\n\n";
            else {
                std::cout << "\nBad, I can not make certificates trusted. Maybe the password was wrong? \nError:" + sError + "\n\n";
                return -1;
            }
        }
        return 0;
    } else if (helpArgs.has("lxd-enable") || helpArgs.has("lxd-disable")) {
        Employees::init({});
        EmploySettings *pSettings = findEmploy<EmploySettings>();
        std::string lxd_mode;
        if (helpArgs.has("lxd-enable")) {
            lxd_mode = "enabled";
        } else if (helpArgs.has("lxd-disable")) {
            lxd_mode = "disabled";
        } else {
            std::cout << "\nError with command lxd-enable or lxd-disable\n";
            return -1;
        }
        pSettings->setSettString("lxd_mode", QString::fromStdString(lxd_mode));
        std::cout << "\nCurrent LXD mode: " << pSettings->getSettString("lxd_mode").toStdString() << "\n";
        return 0;
    } else if(helpArgs.has("start") || helpArgs.has("-s")) {
        QThreadPool::globalInstance()->setMaxThreadCount(5);
        WebSocketServer *pServer = new WebSocketServer();
        if(pServer->isFailed()){
            Log::err(TAG, "Could not start server");
            return -1;
        }

        QObject::connect(pServer, &WebSocketServer::closed, &a, &QCoreApplication::quit);
        EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
        // TODO redesign to check config
        QSqlDatabase *db = pDatabase->database();
        if (!db->open()){
            return -1;
        }

        EmployServerConfig *pConfig = findEmploy<EmployServerConfig>();

        // start web server
        
        Log::info(TAG, "Starting web-server on " + std::to_string(pConfig->webPort())
             + " with " + std::to_string(pConfig->webMaxThreads()) + " worker threads");
        g_httpServer.handlers()->add((LightHttpHandlerBase *) new HttpHandlerWebAdminFolder(pConfig->webAdminFolder()));
        g_httpServer.setPort(pConfig->webPort());
        g_httpServer.setMaxWorkers(pConfig->webMaxThreads());
        g_httpServer.start(); // will be block thread*/

        return a.exec();
    }

    helpArgs.printHelp();
    return 0;
}
