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

#include <employ_orchestra.h>
#include <employees.h>
#include <json.hpp>
#include <utility>
#include <utils_lxd.h>

bool UtilsLXDAuth::check_trust_certs(std::string &sError) {
  std::string sUrl = "/1.0";
  nlohmann::json jsonResponse;
  EmployOrchestra *pOrchestra = findWsjcppEmploy<EmployOrchestra>();

  if (!pOrchestra->initConnection()) {
    sError = "Can\'t connect to LXD server";
    return false;
  }

  if (!pOrchestra->send_get_request(sUrl, jsonResponse, sError))
    return false;

  return (jsonResponse["metadata"]["auth"].is_string()) && (jsonResponse["metadata"]["auth"] == "trusted");
}

bool UtilsLXDAuth::connect_with_lxd(const std::string &sPass, std::string &sError) {
  bool bTrusted = check_trust_certs(sError);

  if (!sError.empty()) {
    WsjcppLog::err(std::string("UtilsLXDAuth"), "Can't get info about client cert");
    return false;
  }

  if (!bTrusted) {
    if (!set_trusted(sPass, sError)) {
      WsjcppLog::err("UtilsLXDAuth", "Can't set trusted certs" + sError);
    } else {
      bTrusted = true;
    }
  }

  return bTrusted;
}

bool UtilsLXDAuth::set_trusted(const std::string &sPass, std::string &sError) {
  std::string sUrl = "/1.0/certificates";
  nlohmann::json jsonData = R"(
    {
        "type": "client",
        "password": "change it"
    })"_json;
  jsonData["password"] = sPass;
  nlohmann::json jsonResponse;
  EmployOrchestra *pOrchestra = findWsjcppEmploy<EmployOrchestra>();

  if (!pOrchestra->initConnection()) {
    sError = "Can\'t connect to LXD server";
    return false;
  }

  return pOrchestra->send_post_request(sUrl, jsonData, jsonResponse, sError);
}
