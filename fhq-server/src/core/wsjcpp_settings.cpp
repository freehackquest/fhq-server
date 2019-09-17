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
    m_bReadonly = true; // if in a file then it will be readonly
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

WSJCppSettingItem & WSJCppSettingItem::text(const std::string &sDefaultTextValue) {
    Log::throw_err(TAG, "Not implemented yet");
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "text: already defined another data type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_TEXT;
    m_sDefaultStringValue = sDefaultTextValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::number(int nDefaultNumberValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "number: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_NUMBER;
    m_nDefaultNumberValue = nDefaultNumberValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::boolean(bool bDefaultBooleanValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "boolean: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_BOOLEAN;
    m_bDefaultBooleanValue = bDefaultBooleanValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::json() {
    Log::throw_err(TAG, "Not implemented yet");

    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        Log::throw_err(TAG, "json: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_JSON;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::list() {
    Log::throw_err(TAG, "Not implemented yet");

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
    return m_bInited ? m_sStringValue : m_sDefaultStringValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setStringValue(const std::string &sStringValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
        Log::throw_err(TAG, "setStringValue: Expected type string");
    }
    if (m_bInited && m_bReadonly) {
        Log::throw_err(TAG, "setStringValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_sStringValue = sStringValue;
}

// ----------------------------------------------------------------------

int WSJCppSettingItem::getDefaultNumberValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
        Log::throw_err(TAG, "getDefaultNumberValue: Expected type number");
    }
    return m_nDefaultNumberValue;
}

// ----------------------------------------------------------------------

int WSJCppSettingItem::getNumberValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
        Log::throw_err(TAG, "getNumberValue: Expected type number");
    }
    return m_bInited ? m_nNumberValue : m_nDefaultNumberValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setNumberValue(int nNumberValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
        Log::throw_err(TAG, "setNumberValue: Expected type number");
    }

    if (m_bInited && m_bReadonly) {
        Log::throw_err(TAG, "setNumberValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_nNumberValue = nNumberValue;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::getDefaultBooleanValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
        Log::throw_err(TAG, "getBooleanValue: Expected type boolean");
    }
    return m_bDefaultBooleanValue;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::getBooleanValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
        Log::throw_err(TAG, "getBooleanValue: Expected type boolean");
    }
    return m_bInited ? m_bBooleanValue : m_bDefaultBooleanValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setBooleanValue(bool bBooleanValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
        Log::throw_err(TAG, "setBooleanValue: Expected type boolean");
    }

    if (m_bInited && m_bReadonly) {
        Log::throw_err(TAG, "setBooleanValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_bBooleanValue = bBooleanValue;
}