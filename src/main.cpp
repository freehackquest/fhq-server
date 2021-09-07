// Copyright (c) 2015-2020 FreeHackQuest <freehackquest@gmail.com>

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


#include <employees.h>
#include <employ_server_info.h>
#include <employ_database.h>
#include <employ_images.h>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <wsjcpp_light_web_server.h>
#include <http_handler_web_user_folder.h>
#include <http_handler_web_public_folder.h>
#include <http_handler_web_admin_folder.h>
#include <jobs_pool.h>
#include <fallen.h>
#include <wsjcpp_core.h>
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
