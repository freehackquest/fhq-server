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
#include <employ_server_info.h>
#include <employees.h>
#include <iostream>
#include <wsjcpp_core.h>
#include <wsjcpp_storages.h>
#include <wsjcpp_yaml.h>

// ----------------------------------------------------------------------
// WsjcppSettingItem

WsjcppSettingItem::WsjcppSettingItem(const std::string &sSettingGroup, const std::string &sSettingName) {
  TAG = "WsjcppSettingItem-(group:" + sSettingGroup + ")/get(\"" + sSettingName + "\")";
  m_sSettingName = sSettingName;
  m_sSettingGroup = sSettingGroup;
  m_nStorageType = WJSCPP_SETTING_IN_NONE;
  m_sStorageType = "none";
  m_bReadonly = false;
  m_nSettingType = WJSCPP_SETTING_TYPE_NONE;
  m_bInited = false;

  // isString
  m_sDefaultStringValue = "";
  m_sStringValue = "";

  // isNumber
  m_nDefaultNumberValue = 0;
  m_nNumberValue = 0;

  // isBoolean
  m_bDefaultBooleanValue = false;
  m_bBooleanValue = false;
}

// ----------------------------------------------------------------------

/*WsjcppSettingItem::WsjcppSettingItem(const WsjcppSettingItem &item) {
    m_sSettingName = item.getName();
    TAG = "WsjcppSettingItem-" + sSettingName;
    m_bReadonly = item.isReadonly();
}*/

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::inFile() {
  if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
    WsjcppLog::throw_err(TAG, "inFile: already defined another storage type");
  }
  m_nStorageType = WJSCPP_SETTING_IN_FILE;
  m_sStorageType = "file";
  m_bReadonly = true; // if in a file then it will be readonly
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::inDatabase() {
  if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
    WsjcppLog::throw_err(TAG, "inDatabase: already defined another storage type");
  }
  m_nStorageType = WJSCPP_SETTING_IN_DATABASE;
  m_sStorageType = "database";
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::inRuntime() {
  if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
    WsjcppLog::throw_err(TAG, "inRuntime: already defined another storage type");
  }

  m_nStorageType = WJSCPP_SETTING_IN_RUNTIME;
  m_sStorageType = "runtime";
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::readonly() {
  m_bReadonly = true;
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::string(const std::string &sDefaultStringValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "string: already defined another data type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_STRING;
  m_sSettingType = "string";
  m_sDefaultStringValue = sDefaultStringValue;
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::password(const std::string &sDefaultPasswordValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "password: already defined another data type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_PASSWORD;
  m_sSettingType = "password";
  m_sDefaultPasswordValue = sDefaultPasswordValue;
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::dirPath(const std::string &sDefaultDirPathValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "dirPath: already defined another data type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_DIRPATH;
  m_sSettingType = "dir_path";
  m_sDefaultDirPathValue = sDefaultDirPathValue;
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::filePath(const std::string &sDefaultFilePathValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "filePath: already defined another data type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_FILEPATH;
  m_sSettingType = "file_path";
  m_sDefaultFilePathValue = sDefaultFilePathValue;
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::text(const std::string &sDefaultTextValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "text: already defined another data type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_TEXT;
  m_sSettingType = "text";
  m_sDefaultTextValue = sDefaultTextValue;
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::number(int nDefaultNumberValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "number: already defined another storage type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_NUMBER;
  m_sSettingType = "number";
  m_nDefaultNumberValue = nDefaultNumberValue;
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::boolean(bool bDefaultBooleanValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "boolean: already defined another storage type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_BOOLEAN;
  m_sSettingType = "boolean";
  m_bDefaultBooleanValue = bDefaultBooleanValue;
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::json() {
  WsjcppLog::throw_err(TAG, "json, Not implemented yet");

  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "json: already defined another storage type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_JSON;
  m_sSettingType = "json";
  return *this;
}

// ----------------------------------------------------------------------

WsjcppSettingItem &WsjcppSettingItem::list() {
  WsjcppLog::throw_err(TAG, "list, Not implemented yet");

  if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "json: already defined another storage type");
  }
  m_nSettingType = WJSCPP_SETTING_TYPE_LIST;
  m_sSettingType = "list";
  return *this;
}

// ----------------------------------------------------------------------

void WsjcppSettingItem::checkWithThrow() const {
  if (m_nSettingType == WJSCPP_SETTING_TYPE_NONE) {
    WsjcppLog::throw_err(TAG, "check: settings type could be NONE");
  }

  if (m_nStorageType == WJSCPP_SETTING_IN_NONE) {
    WsjcppLog::throw_err(TAG, "check: storage type could not be IN_NONE");
  }

  if (m_sSettingName == "") {
    WsjcppLog::throw_err(TAG, "check: name could not be empty");
  }
}

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isInited() const { return m_bInited; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isReadonly() const { return m_bReadonly; }

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getName() const { return m_sSettingName; }

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getGroupName() const { return m_sSettingGroup; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isFromFile() const { return m_nStorageType == WJSCPP_SETTING_IN_FILE; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isFromDatabase() const { return m_nStorageType == WJSCPP_SETTING_IN_DATABASE; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isFromRuntime() const { return m_nStorageType == WJSCPP_SETTING_IN_RUNTIME; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isString() const { return m_nSettingType == WJSCPP_SETTING_TYPE_STRING; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isPassword() const { return m_nSettingType == WJSCPP_SETTING_TYPE_PASSWORD; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isDirPath() const { return m_nSettingType == WJSCPP_SETTING_TYPE_DIRPATH; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isFilePath() const { return m_nSettingType == WJSCPP_SETTING_TYPE_FILEPATH; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isText() const { return m_nSettingType == WJSCPP_SETTING_TYPE_TEXT; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isNumber() const { return m_nSettingType == WJSCPP_SETTING_TYPE_NUMBER; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isBoolean() const { return m_nSettingType == WJSCPP_SETTING_TYPE_BOOLEAN; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isJson() const { return m_nSettingType == WJSCPP_SETTING_TYPE_JSON; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isList() const { return m_nSettingType == WJSCPP_SETTING_TYPE_LIST; }

// ----------------------------------------------------------------------

bool WsjcppSettingItem::isLikeString() const {
  return m_nSettingType == WJSCPP_SETTING_TYPE_STRING || m_nSettingType == WJSCPP_SETTING_TYPE_PASSWORD ||
         m_nSettingType == WJSCPP_SETTING_TYPE_DIRPATH || m_nSettingType == WJSCPP_SETTING_TYPE_FILEPATH ||
         m_nSettingType == WJSCPP_SETTING_TYPE_TEXT;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getDefaultStringValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
    WsjcppLog::throw_err(TAG, "getDefaultStringValue: Expected type string");
  }
  return m_sDefaultStringValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getStringValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
    WsjcppLog::throw_err(TAG, "getStringValue: Expected type string");
  }
  return m_bInited ? m_sStringValue : m_sDefaultStringValue;
}

// ----------------------------------------------------------------------

void WsjcppSettingItem::setStringValue(const std::string &sStringValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
    WsjcppLog::throw_err(TAG, "setStringValue: Expected type string");
  }
  if (m_bInited && m_bReadonly) {
    WsjcppLog::throw_err(TAG, "setStringValue: Could not change value for readonly setting");
  }
  m_bInited = true;
  m_sStringValue = sStringValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getDefaultPasswordValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_PASSWORD) {
    WsjcppLog::throw_err(TAG, "getDefaultPasswordValue: Expected type password");
  }
  return m_sDefaultPasswordValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getPasswordValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_PASSWORD) {
    WsjcppLog::throw_err(TAG, "getPasswordValue: Expected type password");
  }
  return m_bInited ? m_sPasswordValue : m_sDefaultPasswordValue;
}

// ----------------------------------------------------------------------

void WsjcppSettingItem::setPasswordValue(const std::string &sPasswordValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_PASSWORD) {
    WsjcppLog::throw_err(TAG, "setStringValue: Expected type password");
  }
  if (m_bInited && m_bReadonly) {
    WsjcppLog::throw_err(TAG, "setPasswordValue: Could not change value for readonly setting");
  }
  m_bInited = true;
  m_sPasswordValue = sPasswordValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getDefaultDirPathValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_DIRPATH) {
    WsjcppLog::throw_err(TAG, "getDefaultDirPathValue: Expected type dirpath");
  }
  return m_sDefaultDirPathValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getDirPathValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_DIRPATH) {
    WsjcppLog::throw_err(TAG, "getDirPathValue: Expected type dirpath");
  }
  return m_bInited ? m_sDirPathValue : m_sDefaultDirPathValue;
}

// ----------------------------------------------------------------------

void WsjcppSettingItem::setDirPathValue(const std::string &sDirPathValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_DIRPATH) {
    WsjcppLog::throw_err(TAG, "setDirPathValue: Expected type dirpath");
  }
  if (m_bInited && m_bReadonly) {
    WsjcppLog::throw_err(TAG, "setDirPathValue: Could not change value for readonly setting");
  }
  m_bInited = true;
  m_sDirPathValue = sDirPathValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getDefaultFilePathValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_FILEPATH) {
    WsjcppLog::throw_err(TAG, "getDefaultFilePathValue: Expected type filepath");
  }
  return m_sDefaultFilePathValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getFilePathValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_FILEPATH) {
    WsjcppLog::throw_err(TAG, "getFilePathValue: Expected type filepath");
  }
  return m_bInited ? m_sFilePathValue : m_sDefaultFilePathValue;
}

// ----------------------------------------------------------------------

void WsjcppSettingItem::setFilePathValue(const std::string &sFilePathValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_FILEPATH) {
    WsjcppLog::throw_err(TAG, "setFilePathValue: Expected type filepath");
  }
  if (m_bInited && m_bReadonly) {
    WsjcppLog::throw_err(TAG, "setFilePathValue: Could not change value for readonly setting");
  }
  m_bInited = true;
  m_sFilePathValue = sFilePathValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getDefaultTextValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_TEXT) {
    WsjcppLog::throw_err(TAG, "getDefaultTextValue: Expected type text");
  }
  return m_sDefaultTextValue;
}

// ----------------------------------------------------------------------

std::string WsjcppSettingItem::getTextValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_TEXT) {
    WsjcppLog::throw_err(TAG, "getTextValue: Expected type text");
  }
  return m_bInited ? m_sTextValue : m_sDefaultTextValue;
}

// ----------------------------------------------------------------------

void WsjcppSettingItem::setTextValue(const std::string &sTextValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_TEXT) {
    WsjcppLog::throw_err(TAG, "setTextValue: Expected type text");
  }
  if (m_bInited && m_bReadonly) {
    WsjcppLog::throw_err(TAG, "setTextValue: Could not change value for readonly setting");
  }
  m_bInited = true;
  m_sTextValue = sTextValue;
}

// ----------------------------------------------------------------------

int WsjcppSettingItem::getDefaultNumberValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
    WsjcppLog::throw_err(TAG, "getDefaultNumberValue: Expected type number");
  }
  return m_nDefaultNumberValue;
}

// ----------------------------------------------------------------------

int WsjcppSettingItem::getNumberValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
    WsjcppLog::throw_err(TAG, "getNumberValue: Expected type number");
  }
  return m_bInited ? m_nNumberValue : m_nDefaultNumberValue;
}

// ----------------------------------------------------------------------

void WsjcppSettingItem::setNumberValue(int nNumberValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
    WsjcppLog::throw_err(TAG, "setNumberValue: Expected type number");
  }

  if (m_bInited && m_bReadonly) {
    WsjcppLog::throw_err(TAG, "setNumberValue: Could not change value for readonly setting");
  }
  m_bInited = true;
  m_nNumberValue = nNumberValue;
}

// ----------------------------------------------------------------------

bool WsjcppSettingItem::getDefaultBooleanValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
    WsjcppLog::throw_err(TAG, "getBooleanValue: Expected type boolean");
  }
  return m_bDefaultBooleanValue;
}

// ----------------------------------------------------------------------

bool WsjcppSettingItem::getBooleanValue() const {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
    WsjcppLog::throw_err(TAG, "getBooleanValue: Expected type boolean");
  }
  return m_bInited ? m_bBooleanValue : m_bDefaultBooleanValue;
}

// ----------------------------------------------------------------------

void WsjcppSettingItem::setBooleanValue(bool bBooleanValue) {
  if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
    WsjcppLog::throw_err(TAG, "setBooleanValue: Expected type boolean");
  }

  if (m_bInited && m_bReadonly) {
    WsjcppLog::throw_err(TAG, "setBooleanValue: Could not change value for readonly setting");
  }
  m_bInited = true;
  m_bBooleanValue = bBooleanValue;
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppSettingItem::toJson(bool bHidePassword) {
  nlohmann::json jsonSett;
  jsonSett["name"] = m_sSettingName;
  jsonSett["group"] = m_sSettingGroup;
  jsonSett["readonly"] = m_bReadonly;
  jsonSett["inited"] = m_bInited;
  jsonSett["storage_type"] = m_sStorageType;
  jsonSett["type"] = m_sSettingType;
  if (m_nSettingType == WJSCPP_SETTING_TYPE_BOOLEAN) {
    jsonSett["value"] = getBooleanValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_STRING) {
    jsonSett["value"] = getStringValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_NUMBER) {
    jsonSett["value"] = getNumberValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_PASSWORD) {
    jsonSett["value"] = bHidePassword ? "******" : getPasswordValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_DIRPATH) {
    jsonSett["value"] = getDirPathValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_FILEPATH) {
    jsonSett["value"] = getFilePathValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_TEXT) {
    jsonSett["value"] = getTextValue();
  } else {
    jsonSett["value"] = "unknown";
    jsonSett["type"] = "not_implemented_yet";
  }
  return jsonSett;
}

// ---------------------------------------------------------------------

std::string WsjcppSettingItem::convertValueToString(bool bHidePassword) const {
  std::string sRet;
  if (m_nSettingType == WJSCPP_SETTING_TYPE_BOOLEAN) {
    sRet = getBooleanValue() ? "yes" : "no";
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_STRING) {
    sRet = getStringValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_NUMBER) {
    sRet = std::to_string(getNumberValue());
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_PASSWORD) {
    sRet = bHidePassword ? "(hidden)" : getPasswordValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_DIRPATH) {
    sRet = getDirPathValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_FILEPATH) {
    sRet = getFilePathValue();
  } else if (m_nSettingType == WJSCPP_SETTING_TYPE_TEXT) {
    sRet = getTextValue();
  } else {
    WsjcppLog::throw_err(TAG, "convertValueToString, Not implemented type of setting");
  }
  return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppSettingItem::convertTypeToString() const { return m_sSettingType; }

// ---------------------------------------------------------------------
// EmployGlobalSettings

REGISTRY_WJSCPP_EMPLOY(EmployGlobalSettings)

// ---------------------------------------------------------------------

EmployGlobalSettings::EmployGlobalSettings() : WsjcppEmployBase(EmployGlobalSettings::name(), {}) {

  TAG = EmployGlobalSettings::name();

  // basicly
  this->registrySetting("runtime", "work_dir").dirPath("").inRuntime().readonly();
  this->registrySetting("runtime", "log_dir").dirPath("").inRuntime().readonly();
  this->registrySetting("runtime", "app_version").string("").inRuntime().readonly();
  this->registrySetting("runtime", "app_name").string("").inRuntime().readonly();
  this->registrySetting("runtime", "app_author").string("").inRuntime().readonly();
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::EmployGlobalSettings::init() {
  // checking settings
  WsjcppLog::info(TAG, "EmployGlobalSettings");
  std::map<std::string, WsjcppSettingItem *>::iterator it;
  for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
    WsjcppSettingItem *pItem = it->second;
    pItem->checkWithThrow();
  }
  return this->initFromFile();
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::deinit() {
  // TODO
  return true;
}

// ---------------------------------------------------------------------

// TODO redesign to ->update
void EmployGlobalSettings::setWorkDir(const std::string &sWorkDir) { m_sWorkDir = sWorkDir; }

// ---------------------------------------------------------------------

WsjcppSettingItem &EmployGlobalSettings::registrySetting(const std::string &sSettingGroup,
                                                         const std::string &sSettingName) {
  std::map<std::string, WsjcppSettingItem *>::iterator it;
  it = m_mapSettingItems.find(sSettingName);
  if (it != m_mapSettingItems.end()) {
    WsjcppLog::throw_err(TAG, "Setting '" + sSettingName + "' already registered");
  }
  WsjcppSettingItem *pItem = new WsjcppSettingItem(sSettingGroup, sSettingName);
  m_mapSettingItems.insert(std::pair<std::string, WsjcppSettingItem *>(sSettingName, pItem));
  return *pItem;
}

// ---------------------------------------------------------------------

const WsjcppSettingItem &EmployGlobalSettings::get(const std::string &sSettingName) {
  std::map<std::string, WsjcppSettingItem *>::iterator it;
  it = m_mapSettingItems.find(sSettingName);
  if (it == m_mapSettingItems.end()) {
    WsjcppLog::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
  }
  return *(it->second);
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::exists(const std::string &sSettingName) {
  std::map<std::string, WsjcppSettingItem *>::iterator it;
  it = m_mapSettingItems.find(sSettingName);
  return it != m_mapSettingItems.end();
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::update(const std::string &sSettingName, const std::string &sValue) {
  std::map<std::string, WsjcppSettingItem *>::iterator it;
  it = m_mapSettingItems.find(sSettingName);
  if (it == m_mapSettingItems.end()) {
    WsjcppLog::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
  }
  WsjcppSettingItem *pItem = it->second;
  if (pItem->isString()) {
    pItem->setStringValue(sValue);
  } else if (pItem->isPassword()) {
    pItem->setPasswordValue(sValue);
  } else if (pItem->isFilePath()) {
    pItem->setFilePathValue(sValue);
  } else if (pItem->isDirPath()) {
    pItem->setDirPathValue(sValue);
  } else if (pItem->isText()) {
    pItem->setTextValue(sValue);
  } else if (pItem->isBoolean()) {
    pItem->setBooleanValue(sValue == "yes");
  } else {
    WsjcppLog::throw_err(TAG, "Error on updating setting '" + sSettingName + "' ");
  }
  if (pItem->isFromDatabase()) {
    m_pDatabaseSettingsStore->updateSettingItem(pItem);
  }
  // TODO send to listeners
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::update(const std::string &sSettingName, int nValue) {
  std::map<std::string, WsjcppSettingItem *>::iterator it;
  it = m_mapSettingItems.find(sSettingName);
  if (it == m_mapSettingItems.end()) {
    WsjcppLog::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
  }
  WsjcppSettingItem *pItem = it->second;
  if (pItem->isNumber()) {
    pItem->setNumberValue(nValue);
  } else {
    WsjcppLog::throw_err(TAG, "Error on updating number setting '" + sSettingName + "' ");
  }
  if (pItem->isFromDatabase()) {
    m_pDatabaseSettingsStore->updateSettingItem(pItem);
  }
  // TODO send to listeners
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::update(const std::string &sSettingName, bool bValue) {
  std::map<std::string, WsjcppSettingItem *>::iterator it;
  it = m_mapSettingItems.find(sSettingName);
  if (it == m_mapSettingItems.end()) {
    WsjcppLog::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
  }
  WsjcppSettingItem *pItem = it->second;
  if (pItem->isBoolean()) {
    pItem->setBooleanValue(bValue);
  } else {
    WsjcppLog::throw_err(TAG, "Error on updating boolean setting '" + sSettingName + "' ");
  }
  if (pItem->isFromDatabase()) {
    m_pDatabaseSettingsStore->updateSettingItem(pItem);
  }
  // TODO send to listeners
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::initFromDatabase(WsjcppSettingsStore *pDatabaseSettingsStore) {
  m_pDatabaseSettingsStore = pDatabaseSettingsStore;
  std::map<std::string, WsjcppSettingItem *>::iterator it;

  std::map<std::string, std::string> vMap = pDatabaseSettingsStore->loadAllSettings();
  std::map<std::string, std::string>::iterator itDb;
  for (itDb = vMap.begin(); itDb != vMap.end(); ++itDb) {
    std::string sName = itDb->first;
    std::string sValue = itDb->second;
    it = m_mapSettingItems.find(sName);
    if (it == m_mapSettingItems.end()) {
      WsjcppLog::warn(TAG, "Excess setting in database, name '" + sName + "'");
    } else {
      WsjcppSettingItem *pItem = it->second;
      if (pItem->isString()) {
        pItem->setStringValue(sValue);
        WsjcppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
      } else if (pItem->isPassword()) {
        pItem->setPasswordValue(sValue);
        WsjcppLog::info(TAG, "Applyed settings from database: " + sName + " = (hidden)");
      } else if (pItem->isBoolean()) {
        // TODO temporary
        if (sName == "allow_quests_proposals") {
          continue;
        }
        pItem->setBooleanValue(sValue == "yes");
        WsjcppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
      } else if (pItem->isDirPath()) {
        pItem->setDirPathValue(sValue);
        WsjcppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
        // TODO check folder ?
      } else if (pItem->isFilePath()) {
        pItem->setFilePathValue(sValue);
        WsjcppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
      } else if (pItem->isNumber()) {
        int nValue = std::stoi(sValue);
        pItem->setNumberValue(nValue);
        WsjcppLog::info(TAG, "Applyed settings from database: " + sName + " = " + std::to_string(nValue));
      } else if (pItem->isText()) {
        pItem->setTextValue(sValue);
        WsjcppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
      } else {
        WsjcppLog::throw_err(TAG, "type of setting (from database) not match with delared, name '" + sName + "'");
      }
    }
  }

  // check what not init yet in database
  for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
    WsjcppSettingItem *pItem = it->second;
    if (pItem->isFromDatabase() && !pItem->isInited()) {
      m_pDatabaseSettingsStore->initSettingItem(pItem);
    }
  }
  return true;
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::printSettings() const {
  std::map<std::string, WsjcppSettingItem *>::const_iterator it;

  for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
    std::string sName = it->first;
    WsjcppSettingItem *pItem = it->second;

    std::cout << " * [" << pItem->getName() << "] => [" << pItem->convertValueToString(true) << "]" << std::endl;
    // jsonSett["group"] = pServerSettHelper->group();
    // jsonSett["type"] = pServerSettHelper->type();
  }
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::findFileConfig() {

  std::vector<std::string> vSearchConfigFile;

  if (m_sWorkDir != "") {
    // TODO convert to fullpath
    vSearchConfigFile.push_back(m_sWorkDir);
    if (!WsjcppCore::dirExists(m_sWorkDir)) {
      WsjcppLog::err(TAG, "Not found directory " + m_sWorkDir);
      return false;
    }
  } else {
    // TODO convert to fullpath
    vSearchConfigFile.push_back("/etc/fhq-server/");
  }

  vSearchConfigFile.push_back("./data/");

  for (int i = 0; i < vSearchConfigFile.size(); i++) {
    std::string sDirPath = vSearchConfigFile[i];
    sDirPath = WsjcppCore::doNormalizePath(sDirPath);
    std::string sFilePathConf = sDirPath + "/config.yml";
    sFilePathConf = WsjcppCore::doNormalizePath(sFilePathConf);
    if (WsjcppCore::fileExists(sFilePathConf)) {
      m_sFilepathConf = sFilePathConf;
      m_sWorkDir = sDirPath;
      WsjcppLog::info(TAG, "Found config file '" + sFilePathConf + "'");
      break;
    } else {
      WsjcppLog::warn(TAG, "Not found possible config file '" + sFilePathConf + "'");
    }
  }

  if (m_sFilepathConf == "") {
    WsjcppLog::err(TAG, "Not found config file");
    return false;
  }
  return true;
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::initFromFile() {
  if (!this->findFileConfig()) {
    return false;
  }
  WsjcppYaml yaml;
  std::string sError;
  if (!yaml.loadFromFile(m_sFilepathConf, sError)) {
    WsjcppLog::err(TAG, "Problem with loading file " + m_sFilepathConf);
    WsjcppLog::err(TAG, sError);
    return false;
  }

  WsjcppYamlCursor cur = yaml.getCursor();

  std::map<std::string, WsjcppSettingItem *>::iterator it;
  for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
    WsjcppSettingItem *pItem = it->second;
    if (pItem->isFromFile()) {
      if (pItem->isString()) {
        std::string s = yaml[pItem->getName()].valStr();
        pItem->setStringValue(s);
        WsjcppLog::info(TAG, pItem->getName() + " = " + s);
        // TODO run validators
      } else if (pItem->isPassword()) {
        std::string sPassword = yaml[pItem->getName()].valStr();
        pItem->setPasswordValue(sPassword);
        WsjcppLog::info(TAG, pItem->getName() + " = (hidden)");
      } else if (pItem->isDirPath()) {
        std::string sDirPath = yaml[pItem->getName()].valStr();
        if (sDirPath.length() > 0 && sDirPath[0] != '/') {
          sDirPath = m_sWorkDir + "/" + sDirPath;
          sDirPath = WsjcppCore::doNormalizePath(sDirPath);
        }
        if (!WsjcppCore::dirExists(sDirPath)) {
          WsjcppLog::throw_err(TAG, "Wrong settings '" + pItem->getName() + "', because folder '" + sDirPath +
                                        "' does not exists");
        }
        pItem->setDirPathValue(sDirPath);
        WsjcppLog::info(TAG, pItem->getName() + " = " + sDirPath);
        // TODO run validators
      } else if (pItem->isFilePath()) {
        std::string sFilePath = yaml[pItem->getName()].valStr();
        if (sFilePath.length() > 0 && sFilePath[0] != '/') {
          sFilePath = m_sWorkDir + "/" + sFilePath;
          sFilePath = WsjcppCore::doNormalizePath(sFilePath);
        }
        if (!WsjcppCore::fileExists(sFilePath)) {
          WsjcppLog::throw_err(TAG, "Wrong settings '" + pItem->getName() + "', because file '" + sFilePath +
                                        "' does not exists");
        }
        pItem->setFilePathValue(sFilePath);
        WsjcppLog::info(TAG, pItem->getName() + " = " + sFilePath);
        // TODO run validators
      } else if (pItem->isNumber()) {
        int nValue = yaml[pItem->getName()].valInt();
        pItem->setNumberValue(nValue);
        WsjcppLog::info(TAG, pItem->getName() + " = " + std::to_string(nValue));
        // TODO run validators
      } else if (pItem->isBoolean()) {
        bool bValue = yaml[pItem->getName()].valBool();
        pItem->setBooleanValue(bValue);
        WsjcppLog::info(TAG, pItem->getName() + " = " + (bValue ? "yes" : "no"));
        // TODO run validators
      } else {
        WsjcppLog::throw_err(TAG, "initFromFile, Not implemented yet");
      }
    }
  }

  // missing settings in file
  for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
    WsjcppSettingItem *pItem = it->second;
    if (pItem->isFromFile() && !pItem->isInited()) {
      WsjcppLog::warn(TAG, "Missing setting: '" + pItem->getName() + "' in file '" + m_sFilepathConf + "'");
    }
  }

  std::vector<std::string> vKeys = cur.keys();
  for (int i = 0; i < vKeys.size(); i++) {
    std::string sKey = vKeys[i];
    it = m_mapSettingItems.find(sKey);
    if (it == m_mapSettingItems.end()) {
      WsjcppLog::warn(TAG, "Excess setting: '" + sKey + "' in file '" + m_sFilepathConf + "'");
    }
  }
  return true;
}

// ---------------------------------------------------------------------

std::string EmployGlobalSettings::getFilepathConf() const { return m_sFilepathConf; }

// ---------------------------------------------------------------------

nlohmann::json EmployGlobalSettings::toJson(bool bPasswordHide) {
  nlohmann::json jsonSettings = nlohmann::json::array();

  std::map<std::string, WsjcppSettingItem *>::iterator it = m_mapSettingItems.begin();
  for (; it != m_mapSettingItems.end(); ++it) {
    std::string sName = it->first;
    WsjcppSettingItem *pItem = it->second;
    jsonSettings.push_back(pItem->toJson(bPasswordHide));
  }
  return jsonSettings;
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::eventSettingsChanged(const WsjcppSettingItem *pSettingItem) {
  for (int i = 0; i < m_vListeners.size(); i++) {
    m_vListeners[i]->onSettingsChanged(pSettingItem);
  }
}

// ---------------------------------------------------------------------

REGISTRY_WJSCPP_EMPLOY(EmployServer)

EmployServer::EmployServer()
    : WsjcppEmployBase(EmployServer::name(), {"start_server", EmployGlobalSettings::name(), EmployServerInfo::name()}) {
  TAG = EmployServer::name();
  m_pWebSocketServer = NULL;

  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  pGlobalSettings->registrySetting("ws_server", "port").number(1234).inFile();
  pGlobalSettings->registrySetting("ws_server", "ssl_on").boolean(false).inFile();
  pGlobalSettings->registrySetting("ws_server", "ssl_port").number(4613).inFile();
  pGlobalSettings->registrySetting("ws_server", "ssl_key_file").filePath("/etc/ssl/private/localhost.key").inFile();
  pGlobalSettings->registrySetting("ws_server", "ssl_cert_file").filePath("/etc/ssl/certs/localhost.crt").inFile();
  pGlobalSettings->registrySetting("web_server", "web_port").number(7080).inFile();
  pGlobalSettings->registrySetting("web_server", "web_max_threads").number(4).inFile();
  pGlobalSettings->registrySetting("functional", "allow_quests_proposals").boolean(true).inFile();

  pGlobalSettings->registrySetting("server", "file_storage").dirPath("/usr/share/fhq-server/file_storage").inFile();

  // TODO validators

  pGlobalSettings->registrySetting("google_map", "google_map_api_key").string("some").inDatabase();

  std::string sGroupServerFolders = "server_folders";

  // deprecated
  pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_quests")
      .dirPath("/var/www/html/fhq/files/quests/")
      .inDatabase();
  // deprecated
  pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_quests_url")
      .string("https://freehackquest.com/files/quests/")
      .inDatabase();

  // deprecated
  pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_games")
      .dirPath("/var/www/html/fhq/public/games/")
      .inDatabase();
  // deprecated
  pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_games_url")
      .string("https://freehackquest.com/public/games/")
      .inDatabase();

  // deprecated
  pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_users")
      .dirPath("/var/www/html/fhq/files/quests/")
      .inDatabase();
  // deprecated (moved to public)
  pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_users_url")
      .string("https://freehackquest.com/files/quests/")
      .inDatabase();

  // TODO move to userprofiles
  pGlobalSettings->registrySetting("user_profile", "profile_change_nick").boolean(true).inDatabase();
}

// ---------------------------------------------------------------------

bool EmployServer::init() {
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  // TODO move to validators
  int nWeb_max_threads = pGlobalSettings->get("web_max_threads").getNumberValue();
  if (nWeb_max_threads <= 0) {
    WsjcppLog::err(TAG, "Wrong option 'web_max_threads', values must be more then 0");
    return false;
  }
  if (nWeb_max_threads > 100) {
    WsjcppLog::err(TAG, "Wrong option 'web_max_threads', values must be less then 0");
    return false;
  }

  return true;
}

// ---------------------------------------------------------------------

bool EmployServer::deinit() {
  // TODO
  return true;
}

// ---------------------------------------------------------------------

void EmployServer::setServer(IWebSocketServer *pWebSocketServer) { m_pWebSocketServer = pWebSocketServer; }

// ---------------------------------------------------------------------

void EmployServer::sendToAll(const nlohmann::json &jsonMessage) { m_pWebSocketServer->sendToAll(jsonMessage); }

// ---------------------------------------------------------------------

void EmployServer::sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) {
  m_pWebSocketServer->sendToOne(pClient, jsonMessage);
}

// ---------------------------------------------------------------------

bool EmployServer::validateInputParameters(WsjcppJsonRpc20Error &error, CmdHandlerBase *pCmdHandler,
                                           const nlohmann::json &jsonMessage) {
  try {
    // TODO check extra params

    for (CmdInputDef inDef : pCmdHandler->inputs()) {

      auto itJsonParamName = jsonMessage.find(inDef.getName());
      const auto endJson = jsonMessage.end();
      if (inDef.isRequired() && itJsonParamName == endJson) {
        error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' expected");
        return false;
      }

      if (itJsonParamName != endJson) {
        if (itJsonParamName->is_null()) {
          error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' could not be null");
          return false;
        }

        if (inDef.isInteger()) {
          if (!itJsonParamName->is_number()) {
            error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' must be integer");
            return false;
          }

          int val = *itJsonParamName;
          if (inDef.isMinVal() && val < inDef.getMinVal()) {
            error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' must be more then " +
                                                  std::to_string(inDef.getMinVal()));
            return false;
          }
          if (inDef.isMaxVal() && val > inDef.getMaxVal()) {
            error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' must be less then " +
                                                  std::to_string(inDef.getMaxVal()));
            return false;
          }
        }

        if (inDef.isString()) {
          std::string sVal = itJsonParamName->get_ref<std::string const &>();
          std::string sError;
          const std::vector<WsjcppValidatorStringBase *> vValidators = inDef.listOfValidators();
          for (int i = 0; i < vValidators.size(); i++) {
            if (!vValidators[i]->isValid(sVal, sError)) {
              error = WsjcppJsonRpc20Error(400, "Wrong param '" + inDef.getName() + "': " + sError);
              return false;
            }
          }
        }
      }
    }
    return true;
  } catch (std::exception const &e) {
    error = WsjcppJsonRpc20Error(500, "InternalServerError");
    WsjcppLog::err(TAG, std::string("validateInputParameters, ") + e.what());
    return false;
  }
}
