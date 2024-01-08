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

#pragma once

#include <employees.h>
#include <model_lxd_orchestra.h>

#include <curl/curl.h>
#include <future>
#include <list>
#include <map>
#include <string>

class EmployOrchestra : public WsjcppEmployBase {
public:
  EmployOrchestra();

  static std::string name() { return "EmployOrchestra"; }

  virtual bool init() override;
  virtual bool deinit() override;

  bool initConnection();
  bool create_container(const std::string &sName, std::string &sError);
  bool create_service(const ServiceConfig &serviceReq, std::string &sError);
  bool create_service(const nlohmann::json &jsonConfig, std::string &sError);
  bool check_response(const nlohmann::json &jsonResponse, std::string &sError);
  bool find_container(const std::string &sName, LXDContainer *&pContainer);
  bool find_service(const std::string &sName, ServiceLXD *&pService);
  bool remove_container(const std::string &sName, std::string &sError);
  bool get_all_profiles(std::vector<std::string> &vecProfiles, std::string &sError);
  bool find_profile(const std::string &sName, std::string &sError);
  bool send_post_request_file(
    const std::string &sUrl, const std::string &sFile, std::string &sResponse, std::string &sError
  );
  bool send_post_request(
    const std::string &sUrl, const nlohmann::json &jsonData, nlohmann::json &jsonResponse, std::string &sError
  );
  bool send_patch_request(
    const std::string &sUrl, const nlohmann::json &jsonData, nlohmann::json &jsonResponse, std::string &sError
  );
  bool send_put_request(
    const std::string &sUrl, const nlohmann::json &jsonData, nlohmann::json &jsonResponse, std::string &sError
  );
  bool send_get_request(const std::string &sUrl, nlohmann::json &jsonResponse, std::string &sError);
  bool send_get_request(const std::string &sUrl, std::string &sResponse, std::string &sError);
  bool send_get_request_raw(const std::string &sUrl, std::string &sResponse, std::string &sError);
  bool send_delete_request(const std::string &sUrl, nlohmann::json &jsonResponse, std::string &sError);

  std::list<std::string> registry_names();
  std::string lastError();

private:
  std::map<std::string, LXDContainer *> m_mapContainers;
  std::map<std::string, ServiceLXD *> m_mapServices;
  std::string m_sPathDirLxcSSL;
  std::string m_sLxdAddress;
  std::string m_sLastError;
  std::string TAG;

  bool m_bTrusted;

  bool pull_container_names();
};
