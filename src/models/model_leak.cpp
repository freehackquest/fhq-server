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

#include <iostream>
#include <model_leak.h>

ModelLeak::ModelLeak() {
  TAG = "ModelLeak";
  m_nLocalId = 0;
  m_sUuid = "";
  m_sGameUuid = "";
  m_nGameId = 0;
  m_sName = "";
  m_sContent = "";
  m_nScore = 0;
  m_nSold = 0;
}

// ---------------------------------------------------------------------

int ModelLeak::localId() { return m_nLocalId; }

// ---------------------------------------------------------------------

void ModelLeak::setLocalId(int nLocalId) { m_nLocalId = nLocalId; }

// ---------------------------------------------------------------------

const std::string &ModelLeak::uuid() { return m_sUuid; }

// ---------------------------------------------------------------------

void ModelLeak::setUuid(std::string sUuid) { m_sUuid = sUuid; }

// ---------------------------------------------------------------------

const std::string &ModelLeak::gameUuid() { return m_sGameUuid; }

// ---------------------------------------------------------------------

void ModelLeak::setGameUuid(std::string sGameUuid) { m_sGameUuid = sGameUuid; }

// ---------------------------------------------------------------------

int ModelLeak::gameId() { // deprecated
  return m_nGameId;
}

// ---------------------------------------------------------------------

void ModelLeak::setGameId(int nGameId) { // deprecated
  m_nGameId = nGameId;
}

// ---------------------------------------------------------------------

const std::string &ModelLeak::name() { return m_sName; }

// ---------------------------------------------------------------------

void ModelLeak::setName(std::string sName) { m_sName = sName; }

// ---------------------------------------------------------------------

const std::string &ModelLeak::content() { return m_sContent; }

// ---------------------------------------------------------------------

void ModelLeak::setContent(std::string sContent) { m_sContent = sContent; }

// ---------------------------------------------------------------------

int ModelLeak::score() { return m_nScore; }

// ---------------------------------------------------------------------

void ModelLeak::setScore(int nScore) { m_nScore = nScore; }

// ---------------------------------------------------------------------

const std::string &ModelLeak::created() { return m_sCreated; }

// ---------------------------------------------------------------------

void ModelLeak::setCreated(std::string sCreated) { m_sCreated = sCreated; }

// ---------------------------------------------------------------------

const std::string &ModelLeak::updated() { return m_sUpdated; }

// ---------------------------------------------------------------------

void ModelLeak::setUpdated(std::string sUpdated) { m_sUpdated = sUpdated; }

// ---------------------------------------------------------------------

int ModelLeak::sold() { return m_nSold; }

// ---------------------------------------------------------------------

void ModelLeak::setSold(int nSold) { m_nSold = nSold; }

// ---------------------------------------------------------------------

nlohmann::json ModelLeak::toJson() {
  nlohmann::json jsonLeak;
  jsonLeak["uuid"] = m_sUuid;
  jsonLeak["gameid"] = m_nGameId; // deprecated
  jsonLeak["game_uuid"] = m_sGameUuid;
  jsonLeak["score"] = m_nScore;
  jsonLeak["sold"] = m_nSold;
  jsonLeak["name"] = m_sName;
  jsonLeak["content"] = m_sContent;
  return jsonLeak;
}

// ---------------------------------------------------------------------

void ModelLeak::fillFrom(nlohmann::json &jsonLeak) {
  // TODO trim
  if (jsonLeak["game_uuid"].is_string()) {
    m_sGameUuid = jsonLeak["game_uuid"];
  }

  // leak_uuid - new leak uuid
  if (jsonLeak["uuid"].is_string()) {
    m_sUuid = jsonLeak["uuid"];
  }

  // TODO trim
  if (jsonLeak["name"].is_string()) {
    m_sName = jsonLeak["name"];
  }

  // TODO trim
  if (jsonLeak["content"].is_string()) {
    m_sContent = jsonLeak["content"];
  }

  // score
  if (jsonLeak["score"].is_number_integer()) {
    m_nScore = jsonLeak.at("score");
  }

  // sold
  if (jsonLeak["sold"].is_number_integer()) {
    m_nSold = jsonLeak.at("sold");
  }
}

// ---------------------------------------------------------------------
