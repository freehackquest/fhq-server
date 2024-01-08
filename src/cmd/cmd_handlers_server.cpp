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

#include <QtCore>
#include <cmd_handlers_server.h>
#include <employ_database.h>
#include <employ_files.h>
#include <employ_server_info.h>
#include <iostream>
#include <runtasks.h>

// ---------------------------------------------------------------------
// This handler will be return public info

REGISTRY_CMD(CmdHandlerPublicInfo)

CmdHandlerPublicInfo::CmdHandlerPublicInfo()
  : CmdHandlerBase("public_info", "Method return public information about server") {

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerPublicInfo::handle(ModelRequest *pRequest) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  EmployServerInfo *pServerInfo = findWsjcppEmploy<EmployServerInfo>();
  EmployFiles *pFiles = findWsjcppEmploy<EmployFiles>();

  nlohmann::json jsonResponse;

  nlohmann::json jsonQuests;
  nlohmann::json jsonWinners = nlohmann::json::array();
  nlohmann::json jsonCities = nlohmann::json::array();

  jsonQuests["count"] = pServerInfo->countQuests();
  jsonQuests["attempts"] = pServerInfo->countQuestsAttempt();
  jsonQuests["solved"] = pServerInfo->countQuestsCompleted();

  QSqlDatabase db = *(pDatabase->database());

  int nMoreThen = 2;
  int nCitiesLimit = 20;

  // TODO load from settings
  // nMoreThen
  // nCitiesLimit

  // cities
  // TODO get from cache
  {
    QSqlQuery query(db);
    query.prepare(""
                  "SELECT * FROM ("
                  "     SELECT city, COUNT(*) cnt FROM users WHERE city IS NOT "
                  "NULL AND city <> '' GROUP BY city ORDER BY cnt DESC "
                  ") AS cities "
                  "WHERE cities.cnt > :morethan "
                  "LIMIT 0,:citieslimit");
    query.bindValue(":morethan", nMoreThen);
    query.bindValue(":citieslimit", nCitiesLimit);
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }
    while (query.next()) {
      QSqlRecord record = query.record();
      nlohmann::json city;
      city["cnt"] = record.value("cnt").toInt();
      city["city"] = record.value("city").toString().toHtmlEscaped().toStdString();
      jsonCities.push_back(city);
    }
  }

  // TODO get from cache
  {
    QSqlQuery query(db);
    query.prepare("SELECT u.nick, u.university, u.rating FROM users u WHERE "
                  "u.role = :role ORDER BY u.rating DESC LIMIT 0,10");
    query.bindValue(":role", "user");
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }
    int nPlace = 1;
    while (query.next()) {
      QSqlRecord record = query.record();
      nlohmann::json jsonUser;
      jsonUser["place"] = nPlace;
      jsonUser["nick"] = record.value("nick").toString().toHtmlEscaped().toStdString();
      jsonUser["university"] = record.value("university").toString().toHtmlEscaped().toStdString();
      jsonUser["rating"] = record.value("rating").toInt();
      jsonWinners.push_back(jsonUser);
      nPlace++;
    }
  }

  jsonResponse["quests"] = jsonQuests;
  jsonResponse["winners"] = jsonWinners;
  jsonResponse["cities"] = jsonCities;
  jsonResponse["files_count"] = pFiles->getCountOfFiles();
  jsonResponse["files_size"] = pFiles->getSizeOfFiles();
  jsonResponse["files_human_size"] = WsjcppCore::getHumanSizeBytes(pFiles->getSizeOfFiles());
  jsonResponse["files_downloads"] = pFiles->getAllDownloadsCounter();

  jsonResponse["developers"] = pServerInfo->developers();
  jsonResponse["connectedusers"] = pRequest->server()->getConnectedUsers();
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// This handler will be return private server info

REGISTRY_CMD(CmdHandlerServerInfo)

CmdHandlerServerInfo::CmdHandlerServerInfo() : CmdHandlerBase("server_info", "Return server private information") {
  TAG = "CmdServerInfoHandler";

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerServerInfo::handle(ModelRequest *pRequest) {
  EmployServerInfo *pServerInfo = findWsjcppEmploy<EmployServerInfo>();
  nlohmann::json jsonResponse;
  nlohmann::json data;

  nlohmann::json jsonRequestStatistics = pServerInfo->toJson(); // TODO how much db connections and time

  data["request_statistics"] = jsonRequestStatistics;
  data["server_started"] = pServerInfo->getServerStart(); // in millseconds
  long updatime = WsjcppCore::getCurrentTimeInMilliseconds();
  updatime = updatime - pServerInfo->getServerStart(); // in seconds
  data["server_uptime_sec"] = updatime / 1000;
  // nlohmann::json lastLogMessages = nlohmann::json::array();
  data["last_log_messages"] = WsjcppLog::getLastLogMessages();
  jsonResponse["data"] = data;

  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// This handler will be return private server settings

REGISTRY_CMD(CmdHandlerServerSettings)

CmdHandlerServerSettings::CmdHandlerServerSettings() : CmdHandlerBase("server_settings", "Return server settings") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerServerSettings::handle(ModelRequest *pRequest) {
  nlohmann::json jsonResponse;

  EmployGlobalSettings *pGloablSettings = findWsjcppEmploy<EmployGlobalSettings>();

  jsonResponse["data"] = pGloablSettings->toJson(true); // TODO how much db connections and time
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// This handler for update private server settings

REGISTRY_CMD(CmdHandlerServerSettingsUpdate)

CmdHandlerServerSettingsUpdate::CmdHandlerServerSettingsUpdate()
  : CmdHandlerBase("server_settings_update", "Update server settings") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  requireStringParam("name", "name of setting");
  requireStringParam("value", "value of setting");
}

// ---------------------------------------------------------------------

void CmdHandlerServerSettingsUpdate::handle(ModelRequest *pRequest) {
  nlohmann::json jsonResponse;

  std::string sName = pRequest->getInputString("name", "");
  std::string sValue = pRequest->getInputString("value", "");

  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  if (!pGlobalSettings->exists(sName)) {
    std::string sError = "Setting with name: " + sName + " did not found";
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, sError));
    return;
  }

  const WsjcppSettingItem sett = pGlobalSettings->get(sName);
  if (sett.isReadonly()) {
    std::string sError = "Setting with name: " + sName + " readonly";
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, sError));
    return;
  }

  std::string sPrevValue = sett.convertValueToString(true);
  if (sett.isLikeString()) {
    pGlobalSettings->update(sName, sValue);
  } else if (sett.isNumber()) {
    pGlobalSettings->update(sName, QString::fromStdString(sValue).toInt());
  } else if (sett.isBoolean()) {
    pGlobalSettings->update(sName, sValue == "yes");
  } else {
    std::string sError = "Setting with name: " + sName + " unknown type";
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, sError));
    return;
  }
  std::string sNewValue = pGlobalSettings->get(sName).convertValueToString(true);
  WsjcppLog::info(TAG, "Settings '" + sName + "' updated from '" + sPrevValue + "' -> '" + sNewValue + "'");
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
