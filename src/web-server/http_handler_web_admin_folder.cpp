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

#include "http_handler_web_admin_folder.h"

#include <wsjcpp_core.h>

HttpHandlerWebAdminFolder::HttpHandlerWebAdminFolder(const std::string &sWebFolder)
  : WsjcppLightWebHttpHandlerBase("web-admin-folder") {

  TAG = "HttpHandlerWebAdminFolder";
  m_sWebFolder = sWebFolder;
}

bool HttpHandlerWebAdminFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
  std::string sRequestPath = pRequest->getRequestPath();

  if (sRequestPath == "") {
    WsjcppLog::err(_tag, "Request path is empty");
    return false;
  }

  if (sRequestPath == "/admin") {
    sRequestPath = "/admin/";
  }

  if (sRequestPath == "/admin/") {
    sRequestPath = "/admin/index.html";
  }

  std::string s = "/admin/";
  if (sRequestPath.rfind("/admin/", 0) != 0) {
    return false;
  }

  if (!WsjcppCore::dirExists(m_sWebFolder)) {
    WsjcppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
  }
  sRequestPath = sRequestPath.substr(6); // remove /admin

  std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
  // WsjcppLog::warn(_tag, "Response Resources " + sFilePath);
  if (!WsjcppCore::fileExists(sFilePath)) { // TODO check the file exists not dir
    return false;
  }
  return true;
}

bool HttpHandlerWebAdminFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  std::string sRequestPath = pRequest->getRequestPath();
  // WsjcppLog::warn(_tag, pRequest->requestPath());

  if (sRequestPath == "/admin") {
    sRequestPath = "/admin/";
  }

  if (sRequestPath == "/admin/") {
    sRequestPath = "/admin/index.html";
  }

  sRequestPath = sRequestPath.substr(6); // remove /admin

  std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
  if (!WsjcppCore::fileExists(sFilePath)) {
    // std::string sResPath = "html" + sRequestPath;
    /*if (ResourcesManager::has(sResPath)) {
        // WsjcppLog::warn(_tag, "Response Resources " + sResPath);
        // ResourceFile *pFile = ResourcesManager::get(sResPath);
        // pRequest->responseBuffer(sResPath, pFile->buffer(),
    pFile->bufferSize()); return true;
    }*/
    return false;
  }

  // TODO 1. if file exists and last date change more that in cache so need
  // update
  // TODO 2. if file not exists but in in resources - response them
  WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
  // WsjcppLog::warn(_tag, "Response File " + sFilePath);
  resp.cacheSec(60).ok().sendFile(sFilePath);
  return true;
}
