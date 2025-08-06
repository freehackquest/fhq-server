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

#include "employ_uuids.h"

#include <algorithm>
#include <employ_database.h>
#include <employ_notify.h>
#include <employees.h>

REGISTRY_WJSCPP_EMPLOY(EmployUuids)

EmployUuids::EmployUuids()
  : WsjcppEmployBase(EmployUuids::name(), {EmployDatabase::name(), EmployGlobalSettings::name()}) {
  TAG = EmployUuids::name();
}

bool EmployUuids::init() {
  WsjcppLog::info(TAG, "Start init defined uuids");

  // predefined allowed types
  addAllowedTypesOfUuid("unknown");
  addAllowedTypesOfUuid("test");

  auto *pDb = findWsjcppEmploy<EmployDatabase>();
  FhqServerDbUuids *pUuids = pDb->databaseUuids();
  m_mapGlobalUuids = pUuids->getAllRecords();
  return true;
}

bool EmployUuids::deinit() {
  m_mapGlobalUuids.clear();
  return true;
}

void EmployUuids::addAllowedTypesOfUuid(const std::string &sTypeOfObject) {
  if (std::find(m_vAllowedTypes.begin(), m_vAllowedTypes.end(), sTypeOfObject) == m_vAllowedTypes.end()) {
    WsjcppLog::info(TAG, "Registered new type of object '" + sTypeOfObject + "'");
    m_vAllowedTypes.push_back(sTypeOfObject);
  } else {
    WsjcppLog::info(TAG, "Already registered type of object '" + sTypeOfObject + "'");
  }
}

const std::vector<std::string> &EmployUuids::getAllowedTypesOfUuid() { return m_vAllowedTypes; }

std::string EmployUuids::generateNewUuid(const std::string &sTypeOfObject) {
  std::lock_guard<std::mutex> lock(m_mutex);
  std::string sUuid = WsjcppCore::createUuid();
  while (m_mapGlobalUuids.count(sUuid)) {
    WsjcppLog::warn(TAG, "Regenerate uuid again");
    sUuid = WsjcppCore::createUuid();
  }
  m_mapGlobalUuids[sUuid] = sTypeOfObject;
  auto *pDb = findWsjcppEmploy<EmployDatabase>();
  if (!pDb->databaseUuids()->insertUuid(sUuid, sTypeOfObject)) {
    WsjcppLog::throw_err(TAG, "Problem with insert to database");
  }
  return sUuid;
}

bool EmployUuids::hasUuid(const std::string &sUuid) { return m_mapGlobalUuids.count(sUuid); }

std::string EmployUuids::getTypeOfObject(const std::string &sUuid) {
  if (m_mapGlobalUuids.count(sUuid)) {
    return m_mapGlobalUuids[sUuid];
  }
  return "unknown";
}
