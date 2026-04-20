/**********************************************************************************
 *                      Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * MIT License
 *
 * Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
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
 * Original repository: https://github.com/freehackquest/fhq-server
 *
 ***********************************************************************************/

#include "argument_processor_config.h"
#include <algorithm>
#include <employees.h>
#include <iomanip>
#include <runtasks.h>
#include <unistd.h> // getpass
#include <websocketserver.h>
#include <wsjcpp_core.h>

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
  registryProcessor(new ArgumentProcessorConfigSet());
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

int ArgumentProcessorConfigTest::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
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
  : WsjcppArgumentProcessor({"print", "ls"}, "Print config", "Print config") {
  TAG = "ArgumentProcessorConfigPrint";
}

int ArgumentProcessorConfigPrint::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
  auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  WsjcppEmployees::init({});
  CmdHandlers::init();
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

int ArgumentProcessorConfigTestEmail::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
  auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  WsjcppEmployees::init({});
  CmdHandlers::init();
  std::cout << "\n * Send test mail\n\n";
  std::string sTo = pGlobalSettings->get("mail_system_message_admin_email").getStringValue();
  std::string sSubject = "Test Mail";
  std::string sContent = "Welcome to Free Hack Quest!\r\n\r\nHow are you?";
  RunTasks::MailSend(sTo, sSubject, sContent);
  RunTasks::waitForDone();
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorConfigSet

ArgumentProcessorConfigSet::ArgumentProcessorConfigSet()
  : WsjcppArgumentProcessor({"set"}, "Set setting value", "Set setting value") {
  TAG = "ArgumentProcessorConfigSet";
  registryExample("./fhq-server cfg set 'mail_username=some@where.org'");
}

int ArgumentProcessorConfigSet::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
  auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  WsjcppEmployees::init({});
  CmdHandlers::init();
  if (vSubParams.size() != 1) {
    return -10; // show help
  }

  std::string sSetting = vSubParams[0];
  std::cout << "\n Try set setting " << sSetting << " \n\n";
  std::string sSettName = "";
  std::istringstream f(sSetting);
  getline(f, sSettName, '=');
  if (sSettName.length() == sSetting.length()) {
    WsjcppLog::err(TAG, "Could not split by '=' for a '" + sSetting + "'");
    return -1;
  }
  std::string sSettValue = sSetting.substr(sSettName.length() + 1);
  if (!pGlobalSettings->exists(sSettName)) {
    WsjcppLog::err(TAG, "Not support settings with name '" + sSettName + "'");
    return -1;
  }

  WsjcppSettingItem item = pGlobalSettings->get(sSettName);
  if (item.isLikeString()) {
    pGlobalSettings->update(sSettName, sSettValue);
  } else if (item.isBoolean()) {
    if (sSettValue != "true" && sSettValue != "yes" && sSettValue != "false" && sSettValue != "no") {
      WsjcppLog::err(
        TAG, "Expected value boolean (true|yes|false|no), but got '" + sSettValue + "' for '" + sSettName + "'"
      );
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
  return 0;
}
