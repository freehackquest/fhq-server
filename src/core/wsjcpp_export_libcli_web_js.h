/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2025 FreeHackQuest <freehackquest@gmail.com>
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

#include <string>
#include <vector>

namespace WsjCpp {
class ExportLibCliWebJS {
public:
  ExportLibCliWebJS();
  void setPackageName(const std::string &sPackageName);
  void setLibraryName(const std::string &sLibraryName);
  void setAppName(const std::string &sAppName);
  void setAppVersion(const std::string &sAppVersion);
  void setAuthor(const std::string &sAuthor);
  void setPrefixRepositoryURL(const std::string &sPrefixRepositoryURL);
  void exportLib();

private:
  long m_nCurrentTimeInSec;
  std::string m_sCurrentDate;
  std::string m_sPackageName;
  std::string m_sLibraryName;
  std::string m_sAppName;
  std::string m_sAppVersion;
  std::string m_sAuthor;
  std::string m_sPrefixRepositoryURL;
  std::vector<std::string> m_vEvents;

  void exportPackageJson(const std::string &sBasicDir);
  void exportAPImd(const std::string &sBasicDir);
  void exportLibCliWebJSFile(const std::string &sBasicDir);
  void exportLibCliWebServiceTSFile(const std::string &sBasicDir);
  void exportSampleHtmlFile(const std::string &sBasicDir);
  void exportPrepareDirs(const std::string &sBasicDir);
};

} // namespace WsjCpp
