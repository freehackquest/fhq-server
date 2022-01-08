
#include "argument_processor_main.h"

#include <QtCore>
#include <QFile>
#include <QString>

#include <wsjcpp_core.h>
#include <employees.h>

#include "argument_processor_api.h"
#include "argument_processor_database.h"
#include "argument_processor_config.h"
#include <utils_prepare_deb_package.h>
#include <wsjcpp_print_tree.h>
#include <wsjcpp_light_web_server.h>
#include <http_handler_web_user_folder.h>
#include <http_handler_web_public_folder.h>
#include <http_handler_web_admin_folder.h>
#include <websocketserver.h>
#include <employees.h>
#include <employ_server_info.h>
#include <employ_database.h>
#include <employ_images.h>


WsjcppLightWebServer g_httpServer;

// ---------------------------------------------------------------------
// ArgumentProcessorMain

ArgumentProcessorMain::ArgumentProcessorMain(QCoreApplication *pQtApp) 
: WsjcppArgumentProcessor({"main"}, "FreeHackQuest Server", "FreeHackQuest Server") {
    TAG = "ArgumentProcessorMain";
    // registrySingleArgument("--single", "What exactly do this single param?");
    registryParameterArgument("--workdir", "<path>", "workdir");
    registryParameterArgument("-wd", "<path>", "workdir (short)");

    // registryExample("here example of command");
    registryProcessor(new ArgumentProcessorVersion());
    registryProcessor(new ArgumentProcessorStart(pQtApp));
    registryProcessor(new ArgumentProcessorDatabase());
    registryProcessor(new ArgumentProcessorConfig());
    registryProcessor(new ArgumentProcessorApi());
    registryProcessor(new ArgumentProcessorShowEmployees());
    registryProcessor(new ArgumentProcessorPrepareDeb());

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
        return setWorkDir(sValue);
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
        WsjcppLog::err(TAG, "Directory '" + sWorkDir + "' did not exists");
        return false;
    }

    // configure employ
    if (sWorkDir != "") {
        pGlobalSettings->setWorkDir(sWorkDir);
        pGlobalSettings->update("work_dir", sWorkDir);
    }

    std::string sDirLogs = WsjcppCore::doNormalizePath(sWorkDir + "/logs");
    if (!WsjcppCore::dirExists(sDirLogs)) {
        WsjcppCore::makeDir(sDirLogs);
        if (!WsjcppCore::dirExists(sDirLogs)) {
            WsjcppLog::err(TAG, "Could not create directory " + sDirLogs);
            return false;
        }
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

// ---------------------------------------------------------------------
// ArgumentProcessorPrepareDeb

ArgumentProcessorPrepareDeb::ArgumentProcessorPrepareDeb() 
: WsjcppArgumentProcessor({"prepare-deb"}, "TODO Prepare Deb Package", "Prepare Deb Package") {
    TAG = "ArgumentProcessorPrepareDeb";
}

// ---------------------------------------------------------------------

int ArgumentProcessorPrepareDeb::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    UtilsPrepareDebPackage::prepare("","tmpdeb");
    return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorShowEmployees

ArgumentProcessorShowEmployees::ArgumentProcessorShowEmployees() 
: WsjcppArgumentProcessor({"show-employees", "se"}, "Show employees", "Show employees") {
    TAG = "ArgumentProcessorShowEmployees";
}

int ArgumentProcessorShowEmployees::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    // dev
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
}

// ---------------------------------------------------------------------
// ArgumentProcessorStart

ArgumentProcessorStart::ArgumentProcessorStart(QCoreApplication *pQtApp) 
: WsjcppArgumentProcessor({"start", "-s"}, "Show employees", "Show employees") {
    TAG = "ArgumentProcessorStart";
    m_pQtApp = pQtApp;
}

int ArgumentProcessorStart::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    // auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

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

    QObject::connect(pServer, &WebSocketServer::closed, m_pQtApp, &QCoreApplication::quit);
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

    return m_pQtApp->exec();
}
