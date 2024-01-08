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

#ifndef EXPORT_LIBWSJCPPCLI_PY_H
#define EXPORT_LIBWSJCPPCLI_PY_H

#include <string>
#include <vector>

struct ApiGroup {
  ApiGroup(std::string sGroupName, std::string sGroupNameCammelCase) {
    this->sGroupName = sGroupName;
    this->sGroupNameCammelCase = sGroupNameCammelCase;
    this->sClassname = "FreeHackQuestApi" + sGroupNameCammelCase;
    this->sFilename = "freehackquest_api_" + sGroupName;
  }
  std::string sGroupName;
  std::string sGroupNameCammelCase;
  std::string sClassname;
  std::string sFilename;
};

class ExportLibWsjCppCliPy {
public:
  ExportLibWsjCppCliPy();
  void exportLib();

private:
  void exportSetupPy();
  void exportAPImd();
  std::string convertCammelCaseToSnakCase(const std::string &sName, bool bSilent);
  void export__init__py();
  void exportApiGroupsPy();
  void exportClientPy();
  void exportPrepareDirs();

  std::string TAG;
  long m_nCurrentTimeInSec;
  std::string m_sCurrentDate;
  std::vector<ApiGroup> m_vGroups;
};

#endif // EXPORT_LIBWSJCPPCLI_PY_H
