// Copyright (c) 2015-2020 FreeHackQuest <freehackquest@gmail.com>

#include <unistd.h>  // getpass

#include <iostream>
#include <string>


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
#include <websocketserver.h>
#include <utils_prepare_deb_package.h>
#include <utils_lxd.h>
#include <employees.h>
#include <employ_server_info.h>
#include <employ_database.h>
#include <employ_images.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <wsjcpp_export_libcli_web_js.h>
#include <export_libwsjcppcli_py.h>
#include <export_libwsjcppcli_java_android.h>
#include <runtasks.h>
#include <wsjcpp_light_web_server.h>
#include <http_handler_web_user_folder.h>
#include <http_handler_web_public_folder.h>
#include <http_handler_web_admin_folder.h>
#include <jobs_pool.h>
#include <fallen.h>
#include <wsjcpp_core.h>
#include <wsjcpp_print_tree.h>
#include <wsjcpp_arguments.h>
#include "argument_processor_main.h"

WsjcppLightWebServer g_httpServer;

int main(int argc, char** argv) {
    std::string appName(WSJCPP_APP_NAME);
    std::string appVersion(WSJCPP_APP_VERSION);
    std::string appAuthor("FreeHackQuest Team");
    std::string sLibraryNameForExports("fhq");
    WsjcppCore::init(
        argc,
        argv,
        appName,
        appVersion,
        appAuthor,
        sLibraryNameForExports);

    QCoreApplication a(argc, argv);
    std::string TAG = "MAIN";
    WsjcppLog::setPrefixLogFile(appName);
    std::string sLogDir = "/var/log/" + appName;
    if (!WsjcppCore::dirExists(sLogDir)) {
        sLogDir = WsjcppCore::getCurrentDirectory() + "./";
        sLogDir = WsjcppCore::doNormalizePath(sLogDir);
    }
    WsjcppLog::setLogDirectory(sLogDir);

    auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    pGlobalSettings->update("app_name", appName);
    pGlobalSettings->update("app_version", appVersion);
    pGlobalSettings->update("app_author", appAuthor);

    FallenHelpParseArgs helpArgs(argc, argv);
    helpArgs.setAppName(appName);
    helpArgs.setAppVersion(appVersion);

    helpArgs.addHelp("--help", "-h", FallenHelpParseArgType::SINGLE_OPTION, "This help");
    helpArgs.addHelp("export-freehackquest-libclient-py", "-exlp", FallenHelpParseArgType::SINGLE_OPTION, "Export freehackquest-libclient-py (python)");
    helpArgs.addHelp("export-libfhqcli-web-javascript", "-exlwjs", FallenHelpParseArgType::SINGLE_OPTION, "Export freehackquest-libclient-web-js (javascript)");
    helpArgs.addHelp("export-libfhqcli-java-android", "-exljadr", FallenHelpParseArgType::SINGLE_OPTION, "Export libfhqcli-java-android");
    helpArgs.addHelp("show-employees", "-se", FallenHelpParseArgType::SINGLE_OPTION, "Show employees");
    helpArgs.addHelp("show-settings", "-ss", FallenHelpParseArgType::SINGLE_OPTION, "Show settings");
    helpArgs.addHelp("set-setting", "-set", FallenHelpParseArgType::PARAMETER, "Set setting value like 'mail_username=some@where.org'");
    helpArgs.addHelp("send-test-mail", "-stm", FallenHelpParseArgType::SINGLE_OPTION, "Send test mail");
    helpArgs.addHelp("prepare-deb", "-pd", FallenHelpParseArgType::SINGLE_OPTION, "Prepare Deb Package");
    helpArgs.addHelp("check-server-config", "-csc", FallenHelpParseArgType::SINGLE_OPTION, "Check server config");
    helpArgs.addHelp("make-config-linux", "-mcl", FallenHelpParseArgType::SINGLE_OPTION, "Create config file for Linux: /etc/fhq-server/fhq-server.conf");
    helpArgs.addHelp("manual-configure-lxd", "-mclxd", FallenHelpParseArgType::SINGLE_OPTION, "Manual configure HTTPS connection with LXD. \n You need generated SSL cert and key in /etc/fhq-server/lxd");
    helpArgs.addHelp("lxd-enable", "-uplxd", FallenHelpParseArgType::SINGLE_OPTION, "Enable lxd mode");
    helpArgs.addHelp("lxd-disable", "-downlxd", FallenHelpParseArgType::SINGLE_OPTION, "Disable lxd mode");
    helpArgs.addHelp("start", "-s", FallenHelpParseArgType::SINGLE_OPTION, "Start server");

    ArgumentProcessorMain *pMain = new ArgumentProcessorMain();
    WsjcppArguments prog(argc, (const char**)argv, pMain);

    int progExec = prog.exec();
    // std::cout << "\n Workdir (2): " << pMain->getWorkDir() << " \n\n";
    // return prog.exec();
    if (progExec == 0) {
        return 0;
    } else if (helpArgs.has("--help")) {
        helpArgs.printHelp();
        return 0;
    } else if (helpArgs.has("export-freehackquest-libclient-py")) {
        ExportLibWsjCppCliPy *pExportPython = new ExportLibWsjCppCliPy();
        pExportPython->exportLib();
        delete pExportPython;
        return 0;
    } else if (helpArgs.has("export-libfhqcli-web-javascript")) {
        WsjCpp::ExportLibCliWebJS *pExportWebJS = new WsjCpp::ExportLibCliWebJS();
        pExportWebJS->setLibraryName("fhq");
        pExportWebJS->setPackageName("freehackquest-libclient-web-js");
        pExportWebJS->setAuthor("FreeHackQuest Team");
        pExportWebJS->setAppName(appName);
        pExportWebJS->setAppVersion(appVersion);
        pExportWebJS->setPrefixRepositoryURL("https://github.com/freehackquest/");
        pExportWebJS->exportLib();
        delete pExportWebJS;
        return 0;
    } else if (helpArgs.has("export-libfhqcli-java-android")) {
        ExportLibWsjCppCliJavaAndroid *pExportJavaAndroid = new ExportLibWsjCppCliJavaAndroid();
        // pExportJavaAndroid->setLibraryName("libfhqcli-java-android");
        // pExportJavaAndroid->setAuthor("FreeHackQuest Team");
        pExportJavaAndroid->setVersion(std::string(WSJCPP_APP_VERSION));
        // pExportJavaAndroid->setPrefixRepositoryURL("https://github.com/freehackquest/");
        pExportJavaAndroid->exportLib();
        delete pExportJavaAndroid;
        return 0;
    } else if (helpArgs.has("show-employees")) {
        WsjcppPrintTree tree("WsjcppEmployees (" + std::to_string(g_pWsjcppEmployees->size()) + ")");

        std::map<std::string, WsjcppEmployBase*>::iterator it = g_pWsjcppEmployees->begin();
        for (; it != g_pWsjcppEmployees->end(); ++it) {
            std::string sEmployName = it->first;
            WsjcppEmployBase* pEmployBase = it->second;
            tree.addChild(sEmployName);
            tree.switchToLatestChild();
            if (pEmployBase->loadAfter().size() > 0) {
                for (int i = 0; i < pEmployBase->loadAfter().size(); i++) {
                    tree.addChild("after: " + pEmployBase->loadAfter().at(i));
                }
            }
            tree.switchToParent();
        }
        std::cout << tree.printTree() << std::endl;        
        return 0;
    } else if (helpArgs.has("prepare-deb")) {
        UtilsPrepareDebPackage::prepare("","tmpdeb");
        return 0;
    } else if (helpArgs.has("check-server-config")) {
        std::cout << "\n * Check Server Config\n\n";
        if (!pGlobalSettings->init()) {
            std::cout << "\n * FAIL\n\n";
        } else {
            std::cout << "\n * Success\n\n";
        }
        return 0;
    } else if (helpArgs.has("show-settings")) {
        WsjcppEmployees::init({});
        std::cout << "\n * Show settings\n\n";
        pGlobalSettings->printSettings();
        std::cout << "\n * Done\n\n";
        return 0;
    } else if (helpArgs.has("set-setting")) {
        WsjcppEmployees::init({});
        std::string sSetting = helpArgs.option("set-setting");
        std::cout << "\n Try set setting " << sSetting << " \n\n";
        std::string sSettName = "";
        std::istringstream f(sSetting);
        getline(f, sSettName, '=');
        if (sSettName.length() == sSetting.length()) {
            WsjcppLog::err(TAG, "Could not split by '=' for a '" + sSetting + "'");
            return -1;
        }
        std::string sSettValue = sSetting.substr(sSettName.length()+1);
        if (!pGlobalSettings->exists(sSettName)) {
            WsjcppLog::err(TAG, "Not support settings with name '" + sSettName + "'");
            return -1;
        }

        WsjcppSettingItem item = pGlobalSettings->get(sSettName);
        if (item.isLikeString()) {
            pGlobalSettings->update(sSettName, sSettValue);
        } else if (item.isBoolean()) {
            if (sSettValue != "true" && sSettValue != "yes" && sSettValue != "false" && sSettValue != "no") {
                WsjcppLog::err(TAG, "Expected value boolean (true|yes|false|no), but got '" + sSettValue + "' for '" + sSettName + "'");
                return -1;
            }
            pGlobalSettings->update(sSettName, sSettValue == "true" || sSettValue == "yes");
        } else if (item.isNumber()) {
            int nSettValue = std::stoi(sSettValue);
            pGlobalSettings->update(sSettName, nSettValue);
        } else {
            WsjcppLog::err(TAG, "Not support settings datatype with name '" + sSettName + "'");
            return -1;
        }
        return 0;
    } else if (helpArgs.has("send-test-mail")) {
        WsjcppEmployees::init({});
        std::cout << "\n * Send test mail\n\n";
        std::string sTo = pGlobalSettings->get("mail_system_message_admin_email").getStringValue();
        std::string sSubject = "Test Mail";
        std::string sContent = "Welcome to Free Hack Quest!\r\n\r\nHow are you?";
        RunTasks::MailSend(sTo, sSubject, sContent);
        RunTasks::waitForDone();
        return 0;
    } else if (helpArgs.has("manual-configure-lxd")) {
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
    } else if (helpArgs.has("lxd-enable") || helpArgs.has("lxd-disable")) {
        WsjcppEmployees::init({});
        bool bLXDMode;
        if (helpArgs.has("lxd-enable")) {
            bLXDMode = true;
        } else if (helpArgs.has("lxd-disable")) {
            bLXDMode = false;
        } else {
            std::cout << "\nError with command lxd-enable or lxd-disable\n";
            return -1;
        }
        pGlobalSettings->update("lxd_mode", bLXDMode);
        std::cout << "\nCurrent LXD mode: " << pGlobalSettings->get("lxd_mode").convertValueToString(false) << "\n";
        return 0;
    } else if (helpArgs.has("start") || helpArgs.has("-s")) {
        pGlobalSettings->registrySetting("web_server", "web_admin_folder").dirPath("/usr/share/fhq-server/web-admin").inFile();
        pGlobalSettings->registrySetting("web_server", "web_user_folder").dirPath("/usr/share/fhq-server/web-user").inFile();
        pGlobalSettings->registrySetting("web_server", "web_public_folder").dirPath("/usr/share/fhq-server/fhq-web-public").inFile();
        pGlobalSettings->registrySetting("web_server", "web_public_folder_url").string("http://localhost:7080/public/").inFile();
        pGlobalSettings->registrySetting("web_server", "web_fhqjad_store").dirPath("/usr/share/fhq-server/web/fhqjad-store").inFile();
        
        WsjcppEmployees::init({"start_server"});

        QThreadPool::globalInstance()->setMaxThreadCount(5);
        WebSocketServer *pServer = new WebSocketServer(); // here will be init settings
        if (pServer->isFailed()) {
            WsjcppLog::err(TAG, "Could not start server");
            return -1;
        }

        QObject::connect(pServer, &WebSocketServer::closed, &a, &QCoreApplication::quit);
        EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
        // TODO redesign to check config
        QSqlDatabase *db = pDatabase->database();
        if (!db->open()) {
            return -1;
        }

        // TODO move inside server start
        // start web server
        int nWebPort = pGlobalSettings->get("web_port").getNumberValue();
        int nWebMaxThreads = pGlobalSettings->get("web_max_threads").getNumberValue();
        std::string sWebAdminFolder = pGlobalSettings->get("web_admin_folder").getDirPathValue();
        std::string sWebUserFolder = pGlobalSettings->get("web_user_folder").getDirPathValue();
        std::string sWebPublicFolder = pGlobalSettings->get("web_public_folder").getDirPathValue(); // TODO must be declared in server
        std::string sWebPublicFolderUrl = pGlobalSettings->get("web_public_folder_url").getStringValue(); // TODO must be declared in server

        WsjcppLog::info(TAG, "Starting web-server on " + std::to_string(nWebPort)
             + " with " + std::to_string(nWebMaxThreads) + " worker threads");

        g_httpServer.addHandler(new HttpHandlerWebAdminFolder(sWebAdminFolder));
        g_httpServer.addHandler(new HttpHandlerWebPublicFolder(sWebPublicFolder));
        g_httpServer.addHandler(new HttpHandlerWebUserFolder(sWebUserFolder));

        g_httpServer.setPort(nWebPort);
        g_httpServer.setMaxWorkers(nWebMaxThreads);
        g_httpServer.start();  // will be block thread

        return a.exec();
    }

    helpArgs.printHelp();
    return 0;
}
