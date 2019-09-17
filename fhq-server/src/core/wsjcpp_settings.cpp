#include "wsjcpp_settings.h"
#include <fallen.h>

// ----------------------------------------------------------------------
// WSJCppSettingItem

WSJCppSettingItem::WSJCppSettingItem(const std::string &sSettingName) {
    TAG = "WSJCppSettingItem-" + sSettingName;
    m_sSettingName = sSettingName;
    m_nStorageType = WJSCPP_SETTING_IN_NONE;
    m_bReadonly = false;
    m_nSettingType = WJSCPP_SETTING_TYPE_NONE;
    m_bInited = false;
}

// ----------------------------------------------------------------------

/*WSJCppSettingItem::WSJCppSettingItem(const WSJCppSettingItem &item) {
    m_sSettingName = item.getName();
    TAG = "WSJCppSettingItem-" + sSettingName;
    m_bReadonly = item.isReadonly();
}*/

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inFile() {
    if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
        Log::throw_err(TAG, "inFile: already defined another storage type");
    }
    m_nStorageType = WJSCPP_SETTING_IN_FILE;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inDatabase() {
    if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
        Log::throw_err(TAG, "inDatabase: already defined another storage type");
    }
    m_nStorageType = WJSCPP_SETTING_IN_DATABASE;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inRuntime() {
    if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
        Log::throw_err(TAG, "inRuntime: already defined another storage type");
    }

    m_nStorageType = WJSCPP_SETTING_IN_RUNTIME;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::readonly() {
    m_bReadonly = true;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::string(const std::string &sDefaultStringValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "string: already defined another data type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_STRING;
    m_sDefaultStringValue = sDefaultStringValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::text() {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "text: already defined another data type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_TEXT;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::number() {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "number: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_NUMBER;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::boolean() {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "boolean: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_BOOLEAN;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::json() {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "json: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_JSON;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::list() {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "json: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_LIST;
    return *this;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::checkWithThrow() const {
    if (m_nSettingType == WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "check: settings type could be NONE");
    }

    if (m_nStorageType == WJSCPP_SETTING_IN_NONE) {
        Log::throw_err(TAG, "check: storage type could not be IN_NONE");
    }
    
    if (m_sSettingName == "") {
        Log::throw_err(TAG, "check: name could not be empty");
    }
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isInited() const {
    return m_bInited;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isReadonly() const {
    return m_bReadonly; 
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getName() const {
    return m_sSettingName;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isFromFile() const {
    return m_nStorageType == WJSCPP_SETTING_IN_FILE;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isFromDatabase() const {
    return m_nStorageType == WJSCPP_SETTING_IN_DATABASE;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isFromRuntime() const {
    return m_nStorageType == WJSCPP_SETTING_IN_RUNTIME;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isString() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_STRING;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isText() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_TEXT;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isNumber() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_NUMBER;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isBoolean() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_BOOLEAN;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isJson() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_JSON;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isList() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_LIST;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getDefaultStringValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
        Log::throw_err(TAG, "getDefaultStringValue: Expected type string");
    }
    return m_sDefaultStringValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getStringValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
        Log::throw_err(TAG, "getStringValue: Expected type string");
    }
    return m_sStringValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setStringValue(const std::string &sStringValue) {
    if (m_bInited && m_bReadonly) {
        Log::throw_err(TAG, "setStringValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_sStringValue = sStringValue;
}