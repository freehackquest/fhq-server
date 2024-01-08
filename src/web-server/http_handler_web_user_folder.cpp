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

#include "http_handler_web_user_folder.h"

#include <wsjcpp_core.h>

HttpHandlerWebUserFolder::HttpHandlerWebUserFolder(const std::string &sWebFolder)
  : WsjcppLightWebHttpHandlerBase("web-user-folder") {

  TAG = "HttpHandlerWebUserFolder";
  m_sWebFolder = sWebFolder;
}

bool HttpHandlerWebUserFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
  std::string sRequestPath = pRequest->getRequestPath();

  if (sRequestPath == "") {
    sRequestPath = "/";
    WsjcppLog::warn(_tag, "Request path is empty");
  }

  if (sRequestPath == "/") {
    sRequestPath = "/index.html";
  }

  if (!WsjcppCore::dirExists(m_sWebFolder)) {
    WsjcppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
  }
  return true;
}

bool HttpHandlerWebUserFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  std::string sRequestPath = pRequest->getRequestPath();
  // WsjcppLog::warn(_tag, pRequest->requestPath());

  if (sRequestPath == "") {
    sRequestPath = "/";
  }

  std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
  if (WsjcppCore::fileExists(sFilePath)) {
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
    resp.cacheSec(60).ok().sendFile(sFilePath);
  } else {
    std::string sFilePath = m_sWebFolder + "/index.html";
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
    resp.cacheSec(60).ok().sendFile(sFilePath);
  }
  return true;
}
