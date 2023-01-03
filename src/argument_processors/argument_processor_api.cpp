
#include "argument_processor_api.h"
#include <algorithm>
#include <employees.h>
#include <export_libwsjcppcli_java_android.h>
#include <export_libwsjcppcli_py.h>
#include <iomanip>
#include <websocketserver.h>
#include <wsjcpp_core.h>
#include <wsjcpp_export_libcli_web_js.h>

// ---------------------------------------------------------------------
// ArgumentProcessorApi

ArgumentProcessorApi::ArgumentProcessorApi()
    : WsjcppArgumentProcessor({"api"}, "api", "All what depened to api processing") {
  TAG = "ArgumentProcessorApi";
  registryProcessor(new ArgumentProcessorApiList());
  registryProcessor(new ArgumentProcessorApiExportPythonLibrary());
  registryProcessor(new ArgumentProcessorApiExportJavascriptLibrary());
  registryProcessor(new ArgumentProcessorApiExportJavaAndroidLibrary());
}

int ArgumentProcessorApi::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
  WsjcppLog::err(TAG, "Not implemented");
  return -10;
}

// ---------------------------------------------------------------------
// ArgumentProcessorApiList

ArgumentProcessorApiList::ArgumentProcessorApiList()
    : WsjcppArgumentProcessor({"list", "ls"}, "Print list of api handlers", "Print list of api handlers") {
  TAG = "ArgumentProcessorApiList";
}

int ArgumentProcessorApiList::exec(const std::vector<std::string> &vRoutes,
                                   const std::vector<std::string> &vSubParams) {
  std::cout << "\n\n * CmdHandlers (" << g_pCmdHandlers->size() << "):\n";

  struct CmdRow {
    std::string sCommandName;
    bool bAccessUnauth;
    bool bAccessUser;
    bool bAccessAdmin;
  };
  std::vector<CmdRow> vRows;
  int nMaxLenCommandName = 0;

  std::map<std::string, CmdHandlerBase *>::iterator it = g_pCmdHandlers->begin();
  for (; it != g_pCmdHandlers->end(); ++it) {
    std::string sCmd = it->first;
    CmdHandlerBase *pCmdHandlerBase = it->second;
    CmdRow row;
    row.bAccessUnauth = pCmdHandlerBase->accessUnauthorized();
    row.bAccessUser = pCmdHandlerBase->accessUser();
    row.bAccessAdmin = pCmdHandlerBase->accessAdmin();
    row.sCommandName = sCmd;
    nMaxLenCommandName = std::max(nMaxLenCommandName, (int)sCmd.length());
    vRows.push_back(row);
  }

  std::cout << " +-" << std::setfill('-') << std::setw(nMaxLenCommandName) << "-"
            << "-+"
            << "--------+--------+--------+\n";
  std::cout << " | " << std::setfill(' ') << std::setw(nMaxLenCommandName) << "Command Name"
            << " |"
            << " Access | Access | Access |\n";
  std::cout << " +-" << std::setfill('-') << std::setw(nMaxLenCommandName) << "-"
            << "-+"
            << "--------+--------+--------+\n";

  for (unsigned int i = 0; i < vRows.size(); i++) {
    std::cout << " | " << std::setfill(' ') << std::setw(nMaxLenCommandName) << vRows[i].sCommandName << " |";
    std::cout << " " << (vRows[i].bAccessUnauth ? "unauth" : "      ") << " |";
    std::cout << "  " << (vRows[i].bAccessUser ? "user" : "    ") << "  |";
    std::cout << " " << (vRows[i].bAccessAdmin ? "admin" : "     ") << "  |";
    std::cout << "\n";
  }
  std::cout << " +-" << std::setfill('-') << std::setw(nMaxLenCommandName) << "-"
            << "-+"
            << "--------+--------+--------+\n";

  std::cout << "\n\n";
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorApiExportPythonLibrary

ArgumentProcessorApiExportPythonLibrary::ArgumentProcessorApiExportPythonLibrary()
    : WsjcppArgumentProcessor({"export-freehackquest-libclient-py", "exlpy"},
                              "Export freehackquest-libclient-py (python)",
                              "Export freehackquest-libclient-py (python)") {
  TAG = "ArgumentProcessorApiExportPythonLibrary";
}

int ArgumentProcessorApiExportPythonLibrary::exec(const std::vector<std::string> &vRoutes,
                                                  const std::vector<std::string> &vSubParams) {
  ExportLibWsjCppCliPy *pExportPython = new ExportLibWsjCppCliPy();
  pExportPython->exportLib();
  delete pExportPython;
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorApiExportJavascriptLibrary

ArgumentProcessorApiExportJavascriptLibrary::ArgumentProcessorApiExportJavascriptLibrary()
    : WsjcppArgumentProcessor({"export-freehackquest-libclient-web-js", "exlwjs"},
                              "Export freehackquest-libclient-web-js (javascript)",
                              "Export freehackquest-libclient-web-js (javascript)") {
  TAG = "ArgumentProcessorApiExportJavascriptLibrary";
}

int ArgumentProcessorApiExportJavascriptLibrary::exec(const std::vector<std::string> &vRoutes,
                                                      const std::vector<std::string> &vSubParams) {
  std::string appName(WSJCPP_APP_NAME);
  std::string appVersion(WSJCPP_APP_VERSION);

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
}

// ---------------------------------------------------------------------
// ArgumentProcessorApiExportJavaAndroidLibrary

ArgumentProcessorApiExportJavaAndroidLibrary::ArgumentProcessorApiExportJavaAndroidLibrary()
    : WsjcppArgumentProcessor({"export-freehackquest-libclient-java-android", "exljadr"},
                              "Export libfhqcli-java-android", "Export libfhqcli-java-android") {
  TAG = "ArgumentProcessorApiExportJavaAndroidLibrary";
}

int ArgumentProcessorApiExportJavaAndroidLibrary::exec(const std::vector<std::string> &vRoutes,
                                                       const std::vector<std::string> &vSubParams) {

  ExportLibWsjCppCliJavaAndroid *pExportJavaAndroid = new ExportLibWsjCppCliJavaAndroid();
  // pExportJavaAndroid->setLibraryName("libfhqcli-java-android");
  // pExportJavaAndroid->setAuthor("FreeHackQuest Team");
  pExportJavaAndroid->setVersion(std::string(WSJCPP_APP_VERSION));
  // pExportJavaAndroid->setPrefixRepositoryURL("https://github.com/freehackquest/");
  pExportJavaAndroid->exportLib();
  delete pExportJavaAndroid;
  return 0;
}
