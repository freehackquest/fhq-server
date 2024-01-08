/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

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

int ArgumentProcessorApiList::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
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
  : WsjcppArgumentProcessor(
      {"export-freehackquest-libclient-py", "exlpy"},
      "Export freehackquest-libclient-py (python)",
      "Export freehackquest-libclient-py (python)"
    ) {
  TAG = "ArgumentProcessorApiExportPythonLibrary";
}

int ArgumentProcessorApiExportPythonLibrary::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
  ExportLibWsjCppCliPy *pExportPython = new ExportLibWsjCppCliPy();
  pExportPython->exportLib();
  delete pExportPython;
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorApiExportJavascriptLibrary

ArgumentProcessorApiExportJavascriptLibrary::ArgumentProcessorApiExportJavascriptLibrary()
  : WsjcppArgumentProcessor(
      {"export-freehackquest-libclient-web-js", "exlwjs"},
      "Export freehackquest-libclient-web-js (javascript)",
      "Export freehackquest-libclient-web-js (javascript)"
    ) {
  TAG = "ArgumentProcessorApiExportJavascriptLibrary";
}

int ArgumentProcessorApiExportJavascriptLibrary::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
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
  : WsjcppArgumentProcessor(
      {"export-freehackquest-libclient-java-android", "exljadr"},
      "Export libfhqcli-java-android",
      "Export libfhqcli-java-android"
    ) {
  TAG = "ArgumentProcessorApiExportJavaAndroidLibrary";
}

int ArgumentProcessorApiExportJavaAndroidLibrary::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {

  ExportLibWsjCppCliJavaAndroid *pExportJavaAndroid = new ExportLibWsjCppCliJavaAndroid();
  // pExportJavaAndroid->setLibraryName("libfhqcli-java-android");
  // pExportJavaAndroid->setAuthor("FreeHackQuest Team");
  pExportJavaAndroid->setVersion(std::string(WSJCPP_APP_VERSION));
  // pExportJavaAndroid->setPrefixRepositoryURL("https://github.com/freehackquest/");
  pExportJavaAndroid->exportLib();
  delete pExportJavaAndroid;
  return 0;
}
