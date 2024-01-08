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

#include <json.hpp>
#include <string>
#include <validators.h>

const int MAX_FREEHACKQUEST_GAME_FORMAT_VERSION = 1;

class ModelGame {
public:
  ModelGame();
  ~ModelGame();

  int localId() const;
  int getLocalId() const;
  void setLocalId(int nLocalId);
  const std::string &uuid() const;
  const std::string &getUuid() const;
  void setUuid(const std::string &sUuid);
  const std::string &name() const;
  void setName(std::string sName);
  const std::string &description() const;
  void setDescription(std::string sDescription);
  const std::string &state() const;
  void setState(const std::string &sState);
  const std::string &form() const;
  void setForm(std::string sForm);
  const std::string &type() const;
  void setType(std::string sType);
  const std::string &dateStart() const;
  void setDateStart(std::string sDateStart);
  const std::string &dateStop() const;
  void setDateStop(std::string sDateStop);
  const std::string &dateRestart() const;
  void setDateRestart(std::string sDateRestart);
  const std::string &organizators() const;
  void setOrganizators(std::string sOrganizators);
  int maxScore() const;
  void setMaxScore(int nMaxScore);

  void copy(const ModelGame &modelGame);
  ModelGame *clone() const;
  nlohmann::json toJson();
  bool fillFrom(const nlohmann::json &jsonGame, std::string &sError);
  bool fillFromFreeHackQuestFormatVersion1(const nlohmann::json &jsonGame, std::string &sError);

private:
  std::string TAG;
  int m_nLocalId; // deprecated
  int m_nFreeHackQuestGameFormatVersion;
  std::string m_sUuid;
  std::string m_sName;
  std::string m_sDescription;
  std::string m_sState;
  std::string m_sForm;
  std::string m_sType;
  std::string m_sDateStart;
  std::string m_sDateStop;
  std::string m_sDateRestart;
  std::string m_sOrganizators;
  int m_nMaxScore;
  ValidatorGameType *m_pValidatorGameType;
  ValidatorGameState *m_pValidatorGameState;
  ValidatorGameForm *m_pValidatorGameForm;
  WsjcppValidatorUUID *m_pValidatorUuid;

  bool readFieldFreeHackQuestGameFormatVersionFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldUuidRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldNameRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldDescriptionOptionalFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldStateRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldFormRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldTypeRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldDateStartRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldDateStopRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldDateRestartRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldOrganizatorsOptionalFromJson(const nlohmann::json &jsonGame, std::string &sError);
};
