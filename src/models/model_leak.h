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

#include <json.hpp>
#include <string>

class ModelLeak {
public:
  ModelLeak();

  int localId();
  void setLocalId(int nId);
  const std::string &uuid();
  void setUuid(std::string sUuid);
  int gameId();                // deprecated
  void setGameId(int nGameId); // deprecated
  const std::string &gameUuid();
  void setGameUuid(std::string nGameUuid);
  const std::string &name();
  void setName(std::string sName);
  const std::string &content();
  void setContent(std::string sContent);
  int score();
  void setScore(int nScore);
  const std::string &created();
  void setCreated(std::string sCreated);
  const std::string &updated();
  void setUpdated(std::string sUpdated);
  int sold();
  void setSold(int nSold);

  nlohmann::json toJson();
  void fillFrom(nlohmann::json &jsonLeak);

private:
  std::string TAG;
  int m_nLocalId;
  int m_nGameId; // deprecated
  std::string m_sUuid;
  std::string m_sGameUuid;
  std::string m_sName;
  std::string m_sContent;
  int m_nScore;
  std::string m_sCreated;
  std::string m_sUpdated;
  int m_nSold;
};
