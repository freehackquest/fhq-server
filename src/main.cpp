// Copyright (c) 2011-2023 FreeHackQuest <freehackquest@gmail.com>

#include <iostream>
#include <string>

#include <QFile>
#include <QString>
#include <QtCore>

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "argument_processor_main.h"
#include <employees.h>
#include <jobs_pool.h>
#include <wsjcpp_arguments.h>
#include <wsjcpp_core.h>

int main(int argc, char **argv) {
  std::string appName(WSJCPP_APP_NAME);
  std::string appVersion(WSJCPP_APP_VERSION);
  std::string appAuthor("FreeHackQuest Team");
  std::string sLibraryNameForExports("fhq");
  WsjcppCore::init(argc, argv, appName, appVersion, appAuthor, sLibraryNameForExports);

  QCoreApplication qtApp(argc, argv);

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

  ArgumentProcessorMain *pMain = new ArgumentProcessorMain(&qtApp);
  WsjcppArguments prog(argc, (const char **)argv, pMain);
  return prog.exec();
}
