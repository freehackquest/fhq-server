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

#ifndef CMD_HADNLERS_LXD_H
#define CMD_HADNLERS_LXD_H

#include <cmd_handlers.h>
#include <model_lxd_orchestra.h>

// ---------------------------------------------------------------------

class CmdHandlerLXDContainers : public CmdHandlerBase {

public:
  CmdHandlerLXDContainers();
  virtual void handle(ModelRequest *pRequest);

private:
  std::string TAG;

  static void create_container(const std::string &name, std::string &sError, int &nErrorCode);
  static void start_container(const std::string &name, std::string &sError, int &nErrorCode);
  static void stop_container(const std::string &name, std::string &sError, int &nErrorCode);
  static void delete_container(const std::string &name, std::string &sError, int &nErrorCode);
};

// ---------------------------------------------------------------------

class CmdHandlerLXDInfo : public CmdHandlerBase {

public:
  CmdHandlerLXDInfo();
  virtual void handle(ModelRequest *pRequest);
  bool get_state(const std::string &sName, std::string &sError, int &nErrorCode, nlohmann::json &jsonState);
};

// ---------------------------------------------------------------------

class CmdHandlerLXDList : public CmdHandlerBase {

public:
  CmdHandlerLXDList();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerLXDExec : public CmdHandlerBase {

public:
  CmdHandlerLXDExec();
  void handle(ModelRequest *pRequest) override;

  static bool exec_command(const std::string &sName, const std::string &sCommand, std::string &sError, int &nErrorCode,
                           std::string &sOutput);
};

// ---------------------------------------------------------------------

class CmdHandlerLXDFile : public CmdHandlerBase {

public:
  CmdHandlerLXDFile();
  void handle(ModelRequest *pRequest) override;

  void pull_file(LXDContainer *pContainer, const std::string &sPath, std::string &sb64File, std::string &sError,
                 int &nErrorCode, bool &isDirectory);
  bool push_file(LXDContainer *pContainer, const std::string &sPath, const std::string &sb64File, std::string &sError,
                 int &nErrorCode);
  bool delete_file(const std::string &sName, const std::string &sPath, std::string &s_b64Zip);
};

// ---------------------------------------------------------------------

class CmdHandlerLXDOpenPort : public CmdHandlerBase {

public:
  CmdHandlerLXDOpenPort();
  void handle(ModelRequest *pRequest) override;
  bool is_port_valide(const std::string &sProto, const int &nPort, std::string &sError, int &nErrorCode);
};

// ---------------------------------------------------------------------

class CmdHandlerLXDImportService : public CmdHandlerBase {

public:
  CmdHandlerLXDImportService();
  void handle(ModelRequest *pRequest) override;
};

// ---------------------------------------------------------------------

class CmdHandlerLXDImportServiceFromZip : public CmdHandlerBase {
public:
  CmdHandlerLXDImportServiceFromZip();
  void handle(ModelRequest *pRequest) override;
};

// ---------------------------------------------------------------------

class CmdHandlerLXDStartService : public CmdHandlerBase {

public:
  CmdHandlerLXDStartService();
  void handle(ModelRequest *pRequest) override;
};

#endif // CMD_HADNLERS_LXD_H
