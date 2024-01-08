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

#include <algorithm>
#include <iostream>
#include <model_game.h>
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------

ModelGame::ModelGame() {
  TAG = "ModelGame";
  m_nLocalId = 0;
  m_nMaxScore = 0;
  m_nFreeHackQuestGameFormatVersion = 0; // not defined
  m_sUuid = "";
  m_sName = "";
  m_sDescription = "";
  m_sState = "";
  m_sForm = "";
  m_sType = "";
  m_sDateStart = "";
  m_sDateStop = "";
  m_sDateRestart = "";
  m_sOrganizators = "";
  m_pValidatorGameType = new ValidatorGameType();
  m_pValidatorGameState = new ValidatorGameState();
  m_pValidatorGameForm = new ValidatorGameForm();
  m_pValidatorUuid = new WsjcppValidatorUUID();
}

ModelGame::~ModelGame() {
  delete m_pValidatorGameType;
  delete m_pValidatorGameState;
  delete m_pValidatorGameForm;
  delete m_pValidatorUuid;
}

int ModelGame::localId() const { return m_nLocalId; }

int ModelGame::getLocalId() const { return m_nLocalId; }

void ModelGame::setLocalId(int nLocalId) { m_nLocalId = nLocalId; }

const std::string &ModelGame::uuid() const { return m_sUuid; }

const std::string &ModelGame::getUuid() const { return m_sUuid; }

void ModelGame::setUuid(const std::string &sUuid) {
  std::string sError;
  if (!m_pValidatorUuid->isValid(sUuid, sError)) {
    WsjcppLog::err(TAG, "Uuid Game:" + sError);
  }
  m_sUuid = sUuid;
  WsjcppCore::trim(m_sUuid);
}

const std::string &ModelGame::name() const { return m_sName; }

void ModelGame::setName(std::string sName) { m_sName = sName; }

const std::string &ModelGame::description() const { return m_sDescription; }

void ModelGame::setDescription(std::string sDescription) { m_sDescription = sDescription; }

const std::string &ModelGame::state() const { return m_sState; }

void ModelGame::setState(const std::string &sState) {
  std::string sError;
  if (!m_pValidatorGameState->isValid(sState, sError)) {
    WsjcppLog::err(TAG, "Game state unknown:" + sError);
  }
  m_sState = sState;
}

const std::string &ModelGame::form() const { return m_sForm; }

void ModelGame::setForm(std::string sForm) {
  std::string sError;
  if (!m_pValidatorGameForm->isValid(sForm, sError)) {
    WsjcppLog::err(TAG, "Game form unknown: '" + sForm + "' " + sError);
  }
  m_sForm = sForm;
}

const std::string &ModelGame::type() const { return m_sType; }

void ModelGame::setType(std::string sType) {
  std::string sError;
  if (!m_pValidatorGameType->isValid(sType, sError)) {
    WsjcppLog::err(TAG, "Game type unknown:" + sError);
  }
  m_sType = sType;
}

const std::string &ModelGame::dateStart() const { return m_sDateStart; }

void ModelGame::setDateStart(std::string sDateStart) { m_sDateStart = sDateStart; }

const std::string &ModelGame::dateStop() const { return m_sDateStop; }

void ModelGame::setDateStop(std::string sDateStop) { m_sDateStop = sDateStop; }

const std::string &ModelGame::dateRestart() const { return m_sDateRestart; }

void ModelGame::setDateRestart(std::string sDateRestart) { m_sDateRestart = sDateRestart; }

const std::string &ModelGame::organizators() const { return m_sOrganizators; }

void ModelGame::setOrganizators(std::string sOrganizators) { m_sOrganizators = sOrganizators; }

int ModelGame::maxScore() const { return m_nMaxScore; }

void ModelGame::setMaxScore(int nMaxScore) { m_nMaxScore = nMaxScore; }

void ModelGame::copy(const ModelGame &modelGame) {
  this->setLocalId(modelGame.localId());
  this->setUuid(modelGame.uuid());
  this->setName(modelGame.name());
  this->setDescription(modelGame.description());
  this->setState(modelGame.state());
  this->setForm(modelGame.form());
  this->setType(modelGame.type());
  this->setDateStart(modelGame.dateStart());
  this->setDateStop(modelGame.dateStop());
  this->setDateRestart(modelGame.dateRestart());
  this->setOrganizators(modelGame.organizators());
  this->setMaxScore(modelGame.maxScore());
}

ModelGame *ModelGame::clone() const {
  ModelGame *pModel = new ModelGame();
  pModel->setLocalId(this->localId());
  pModel->setUuid(this->uuid());
  pModel->setName(this->name());
  pModel->setDescription(this->description());
  pModel->setState(this->state());
  pModel->setForm(this->form());
  pModel->setType(this->type());
  pModel->setDateStart(this->dateStart());
  pModel->setDateStop(this->dateStop());
  pModel->setDateRestart(this->dateRestart());
  pModel->setOrganizators(this->organizators());
  pModel->setMaxScore(this->maxScore());
  return pModel;
}

nlohmann::json ModelGame::toJson() {
  nlohmann::json jsonGame;
  // format 1
  jsonGame["local_id"] = m_nLocalId; // deprecated
  jsonGame["freehackquest_game_format_version"] = MAX_FREEHACKQUEST_GAME_FORMAT_VERSION;
  jsonGame["uuid"] = m_sUuid;
  jsonGame["name"] = m_sName;
  jsonGame["description"] = m_sDescription;
  jsonGame["state"] = m_sState;
  jsonGame["type"] = m_sType;
  jsonGame["form"] = m_sForm;
  jsonGame["date_start"] = m_sDateStart;
  jsonGame["date_stop"] = m_sDateStop;
  jsonGame["date_restart"] = m_sDateRestart;
  jsonGame["organizators"] = m_sOrganizators;
  jsonGame["maxscore"] = m_nMaxScore;
  return jsonGame;
}

bool ModelGame::fillFrom(const nlohmann::json &jsonGame, std::string &sError) {
  // TODO local_id ?
  // TODO maxScore ?

  if (!readFieldFreeHackQuestGameFormatVersionFromJson(jsonGame, sError)) {
    return false;
  }

  if (m_nFreeHackQuestGameFormatVersion == 1) {
    return fillFromFreeHackQuestFormatVersion1(jsonGame, sError);
  }
  sError = "Not supported 'freehackquest_format_version='" + std::to_string(m_nFreeHackQuestGameFormatVersion);
  WsjcppLog::err(TAG, sError);
  return false;
}

bool ModelGame::fillFromFreeHackQuestFormatVersion1(const nlohmann::json &jsonGame, std::string &sError) {
  bool bRet = true;
  sError = "";

  // uuid, required
  std::string sLocalError;
  if (!readFieldUuidRequiredFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldNameRequiredFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldDescriptionOptionalFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldStateRequiredFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldFormRequiredFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldTypeRequiredFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldDateStartRequiredFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldDateStopRequiredFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldDateRestartRequiredFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!readFieldOrganizatorsOptionalFromJson(jsonGame, sLocalError)) {
    bRet = false;
    sError += sLocalError + "\n";
  }

  if (!bRet) {
    sError = "[GAME] freehackquest_game_format_version=1\n" + sError;
  }
  return bRet;
}

bool ModelGame::readFieldFreeHackQuestGameFormatVersionFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("freehackquest_game_format_version");
  if (it != jsonGame.end()) {
    if (jsonGame["freehackquest_game_format_version"].is_number_integer()) {
      m_nFreeHackQuestGameFormatVersion = jsonGame.at("freehackquest_game_format_version");
    }
  } else {
    sError = "Not found 'freehackquest_game_format_version'";
    WsjcppLog::err(TAG, sError);
    return false;
  }
  return true;
}

bool ModelGame::readFieldUuidRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("uuid");
  if (it == jsonGame.end()) {
    sError = "Missing field 'uuid'\n";
    return false;
  }

  if (!jsonGame["uuid"].is_string()) {
    sError = "Field 'uuid' expected as string\n";
    return false;
  }

  std::string sUuid = jsonGame["uuid"];
  if (!m_pValidatorUuid->isValid(sUuid, sError)) {
    sError = "Wrong format of 'uuid': " + sError;
    return false;
  }
  WsjcppCore::trim(sUuid);
  this->setUuid(sUuid);
  return true;
}

bool ModelGame::readFieldNameRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("name");
  if (it == jsonGame.end()) {
    sError = "Missing field 'name'\n";
    return false;
  }

  if (!jsonGame["name"].is_string()) {
    sError = "Field 'name' expected as string\n";
    return false;
  }

  std::string sName = jsonGame["name"];
  WsjcppCore::trim(sName);
  this->setName(sName);
  return true;
}

bool ModelGame::readFieldDescriptionOptionalFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("description");
  if (it == jsonGame.end()) {
    this->setDescription("");
    return true;
  }

  if (!jsonGame["description"].is_string()) {
    sError = "Field 'description' expected as string\n";
    return false;
  }

  std::string sDescription = jsonGame["description"];
  WsjcppCore::trim(sDescription);
  this->setDescription(sDescription);
  return true;
}

bool ModelGame::readFieldStateRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("state");
  if (it == jsonGame.end()) {
    sError = "Missing field 'state'\n";
    return false;
  }

  if (!jsonGame["state"].is_string()) {
    sError = "Field 'state' expected as string\n";
    return false;
  }

  std::string sState = jsonGame["state"];
  if (!m_pValidatorGameState->isValid(sState, sError)) {
    sError = "Game state unknown: " + sError;
    return false;
  }

  WsjcppCore::trim(sState);
  this->setState(sState);
  return true;
}

bool ModelGame::readFieldFormRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("form");
  if (it == jsonGame.end()) {
    sError = "Missing field 'form'\n";
    return false;
  }

  if (!jsonGame["form"].is_string()) {
    sError = "Field 'form' expected as string\n";
    return false;
  }

  std::string sForm = jsonGame["form"];
  if (!m_pValidatorGameForm->isValid(sForm, sError)) {
    sError = "Game form unknown: " + sError;
    return false;
  }

  WsjcppCore::trim(sForm);
  this->setForm(sForm);
  return true;
}

bool ModelGame::readFieldTypeRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("type");
  if (it == jsonGame.end()) {
    sError = "Missing field 'type'\n";
    return false;
  }

  if (!jsonGame["type"].is_string()) {
    sError = "Field 'type' expected as string\n";
    return false;
  }

  std::string sType = jsonGame["type"];
  if (!m_pValidatorGameType->isValid(sType, sError)) {
    sError = "Game type unknown: " + sError;
    return false;
  }

  WsjcppCore::trim(sType);
  this->setType(sType);
  return true;
}

bool ModelGame::readFieldDateStartRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("date_start");
  if (it == jsonGame.end()) {
    sError = "Missing field 'date_start'\n";
    return false;
  }

  if (!jsonGame["date_start"].is_string()) {
    sError = "Field 'date_start' expected as string\n";
    return false;
  }

  std::string sDateStart = jsonGame["date_start"];
  // TODO validators
  // https://github.com/freehackquest/fhq-server/issues/498
  WsjcppCore::trim(sDateStart);
  this->setDateStart(sDateStart);
  return true;
}

bool ModelGame::readFieldDateStopRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("date_stop");
  if (it == jsonGame.end()) {
    sError = "Missing field 'date_stop'\n";
    return false;
  }

  if (!jsonGame["date_stop"].is_string()) {
    sError = "Field 'date_stop' expected as string\n";
    return false;
  }

  std::string sDateStop = jsonGame["date_stop"];
  // TODO validators
  // https://github.com/freehackquest/fhq-server/issues/498
  WsjcppCore::trim(sDateStop);
  this->setDateStop(sDateStop);
  return true;
}

bool ModelGame::readFieldDateRestartRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  nlohmann::json::const_iterator it = jsonGame.find("date_restart");
  if (it == jsonGame.end()) {
    sError = "Missing field 'date_restart'\n";
    return false;
  }

  if (!jsonGame["date_restart"].is_string()) {
    sError = "Field 'date_restart' expected as string\n";
    return false;
  }

  std::string sDateRestart = jsonGame["date_restart"];
  // TODO validators
  // https://github.com/freehackquest/fhq-server/issues/498
  WsjcppCore::trim(sDateRestart);
  this->setDateRestart(sDateRestart);
  return true;
}

bool ModelGame::readFieldOrganizatorsOptionalFromJson(const nlohmann::json &jsonGame, std::string &sError) {
  // organizators - in format version 1
  // TODO organizers - from format version 2 https://github.com/freehackquest/fhq-server/issues/497
  nlohmann::json::const_iterator it = jsonGame.find("organizators");
  if (it == jsonGame.end()) {
    m_sOrganizators = ""; // optional. it's ok
    return true;
  }

  if (!jsonGame["organizators"].is_string()) {
    sError = "Field 'organizators' expected as string\n";
    return false;
  }

  std::string sOrganizators = jsonGame["organizators"];
  WsjcppCore::trim(sOrganizators);
  this->setOrganizators(sOrganizators);
  return true;
}
