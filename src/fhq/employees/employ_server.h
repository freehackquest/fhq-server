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

#pragma once

#include <core/cmd_handlers.h>
#include <map>
#include <string>
#include <vector>
#include <wsjcpp_employees.h>

// employ enum code results
enum EmployResult {
  OK,
  DATABASE_ERROR,
  ALREADY_EXISTS,
  GAME_NOT_FOUND,
  QUEST_NOT_FOUND,
  LEAK_NOT_FOUND,
  ERROR_NAME_IS_EMPTY,
};

// EmployServer

class EmployServer : public WsjcppEmployBase {
public:
  EmployServer();
  static std::string name() { return "EmployServer"; }
  virtual bool init();
  virtual bool deinit();
  bool
  validateInputParameters(WsjcppJsonRpc20Error &error, CmdHandlerBase *pCmdHandler, const nlohmann::json &jsonMessage);
  void setServer(IWebSocketServer *pWebSocketServer);
  void sendToAll(const nlohmann::json &jsonMessage);
  void sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage);

private:
  std::string TAG;
  IWebSocketServer *m_pWebSocketServer;
};
