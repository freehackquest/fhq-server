#include "wsjcpp_settings.h"

// ----------------------------------------------------------------------
// WSJCppSettingItem

WSJCppSettingItem::WSJCppSettingItem(const std::string &sSettingName) {
    TAG = "WSJCppSettingItem-" + sSettingName;
    m_sSettingName = sSettingName;
    m_nStorageType = WJSCPP_SETTING_IN_RUNTIME;
    m_bReadonly = false;
    m_nSettingType = WJSCPP_SETTING_TYPE_NONE;
}

// ----------------------------------------------------------------------

/*WSJCppSettingItem::WSJCppSettingItem(const WSJCppSettingItem &item) {
    m_sSettingName = item.getName();
    TAG = "WSJCppSettingItem-" + sSettingName;
    m_bReadonly = item.isReadonly();
}*/

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inFile() {
    m_nStorageType = WJSCPP_SETTING_IN_FILE;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inDatabase() {
    m_nStorageType = WJSCPP_SETTING_IN_DATABASE;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inRuntime() {
    m_nStorageType = WJSCPP_SETTING_IN_RUNTIME;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::readonly() {
    m_bReadonly = true;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::string() {
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::text() {
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::number() {
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::boolean() {
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::json() {
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::list() {
    return *this;
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