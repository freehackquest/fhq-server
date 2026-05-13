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

#include <algorithm>
#include <fhq/employees/employ_global_settings.h>
#include <fhq/employees/employ_server_info.h>
#include <wsjcpp_employees.h>
#include <iostream>
#include <wsjcpp_core.h>
#include <wsjcpp_storages.h>
#include <wsjcpp_yaml.h>


// ---------------------------------------------------------------------
// EmployServer

REGISTRY_WJSCPP_EMPLOY(EmployServer)

EmployServer::EmployServer()
  : WsjcppEmployBase(EmployServer::name(), {"start_server", EmployGlobalSettings::name(), EmployServerInfo::name()}) {
  TAG = EmployServer::name();
  m_pWebSocketServer = NULL;

  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  pGlobalSettings->registrySetting("ws_server", "port").number(1234).inFile();
  pGlobalSettings->registrySetting("ws_server", "ssl_on").boolean(false).inFile();
  pGlobalSettings->registrySetting("ws_server", "ssl_port").number(4613).inFile();
  pGlobalSettings->registrySetting("ws_server", "ssl_key_file").filePath("/etc/ssl/private/localhost.key").inFile();
  pGlobalSettings->registrySetting("ws_server", "ssl_cert_file").filePath("/etc/ssl/certs/localhost.crt").inFile();
  pGlobalSettings->registrySetting("web_server", "web_port").number(7080).inFile();
  pGlobalSettings->registrySetting("web_server", "web_max_threads").number(4).inFile();
  pGlobalSettings->registrySetting("functional", "allow_quests_proposals").boolean(true).inFile();

  pGlobalSettings->registrySetting("server", "file_storage").dirPath("/usr/share/fhq-server/file_storage").inFile();

  // TODO validators

  pGlobalSettings->registrySetting("google_map", "google_map_api_key").string("some").inDatabase();

  std::string sGroupServerFolders = "server_folders";

  // deprecated
  pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_games_url")
    .string("https://freehackquest.com/public/games/")
    .inDatabase();

  // TODO move to userprofiles
  pGlobalSettings->registrySetting("user_profile", "profile_change_nick").boolean(true).inDatabase();
}

bool EmployServer::init() {
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  // TODO move to validators
  int nWeb_max_threads = pGlobalSettings->get("web_max_threads").getNumberValue();
  if (nWeb_max_threads <= 0) {
    WsjcppLog::err(TAG, "Wrong option 'web_max_threads', values must be more then 0");
    return false;
  }
  if (nWeb_max_threads > 100) {
    WsjcppLog::err(TAG, "Wrong option 'web_max_threads', values must be less then 0");
    return false;
  }

  return true;
}

bool EmployServer::deinit() {
  // TODO
  return true;
}

void EmployServer::setServer(IWebSocketServer *pWebSocketServer) { m_pWebSocketServer = pWebSocketServer; }

void EmployServer::sendToAll(const nlohmann::json &jsonMessage) { m_pWebSocketServer->sendToAll(jsonMessage); }

void EmployServer::sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) {
  m_pWebSocketServer->sendToOne(pClient, jsonMessage);
}

bool EmployServer::validateInputParameters(
  WsjcppJsonRpc20Error &error, CmdHandlerBase *pCmdHandler, const nlohmann::json &jsonMessage
) {
  try {
    // TODO check extra params

    for (CmdInputDef inDef : pCmdHandler->inputs()) {

      nlohmann::json::const_iterator itJsonParamName = jsonMessage.find(inDef.getName());
      nlohmann::json::const_iterator endJson = jsonMessage.end();
      if (inDef.isRequired() && itJsonParamName == endJson) {
        error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' expected");
        return false;
      }

      if (itJsonParamName != endJson) {
        if (itJsonParamName->is_null()) {
          error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' could not be null");
          return false;
        }

        if (inDef.isInteger()) {
          if (!itJsonParamName->is_number()) {
            error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' must be integer");
            return false;
          }

          int val = *itJsonParamName;
          if (inDef.isMinVal() && val < inDef.getMinVal()) {
            error = WsjcppJsonRpc20Error(
              400, "Parameter '" + inDef.getName() + "' must be more then " + std::to_string(inDef.getMinVal())
            );
            return false;
          }
          if (inDef.isMaxVal() && val > inDef.getMaxVal()) {
            error = WsjcppJsonRpc20Error(
              400, "Parameter '" + inDef.getName() + "' must be less then " + std::to_string(inDef.getMaxVal())
            );
            return false;
          }
        }

        if (inDef.isString()) {
          std::string sVal = itJsonParamName->get_ref<std::string const &>();
          std::string sError;
          const std::vector<WsjcppValidatorStringBase *> vValidators = inDef.listOfValidators();
          for (int i = 0; i < vValidators.size(); i++) {
            if (!vValidators[i]->isValid(sVal, sError)) {
              error = WsjcppJsonRpc20Error(400, "Wrong param '" + inDef.getName() + "': " + sError);
              return false;
            }
          }
        }
      }
    }
    return true;
  } catch (std::exception const &e) {
    error = WsjcppJsonRpc20Error(500, "InternalServerError");
    WsjcppLog::err(TAG, std::string("validateInputParameters, ") + e.what());
    return false;
  }
}
