#ifndef WJSCPP_SETTINGS_H
#define WJSCPP_SETTINGS_H

#include <string>
#include <vector>
#include <regex>

enum WSJCppSettingStorageType {
    WJSCPP_SETTING_IN_FILE,
    WJSCPP_SETTING_IN_DATABASE,
    WJSCPP_SETTING_IN_RUNTIME
};

enum WSJCppSettingDataType {
    WJSCPP_SETTING_TYPE_NONE,
    WJSCPP_SETTING_TYPE_STRING,
    WJSCPP_SETTING_TYPE_TEXT,
    WJSCPP_SETTING_TYPE_NUMBER,
    WJSCPP_SETTING_TYPE_BOOLEAN,
    WJSCPP_SETTING_TYPE_LIST,
    WJSCPP_SETTING_TYPE_JSON
};

// ----------------------------------------------------------------------

class WSJCppSettingItem {
    public:
        WSJCppSettingItem(const std::string &sSettingName);
        // WSJCppSettingItem(const WSJCppSettingItem &item);
        WSJCppSettingItem &inFile();
        WSJCppSettingItem &inDatabase();
        WSJCppSettingItem &inRuntime();
        WSJCppSettingItem &readonly();
        WSJCppSettingItem &string();
        WSJCppSettingItem &text();
        WSJCppSettingItem &number();
        WSJCppSettingItem &boolean();
        WSJCppSettingItem &json();
        WSJCppSettingItem &list();
        
        bool isReadonly() const;
        std::string getName() const;
        bool isFromFile() const;
        bool isFromDatabase() const;
        bool isFromRuntime() const;

    private:
        std::string TAG;
        std::string m_sSettingName;
        bool m_bReadonly;
        WSJCppSettingStorageType m_nStorageType;
        WSJCppSettingDataType m_nSettingType;
};

class WSJCppSettingListener {
    public:
        virtual void eventSettingsChanged() = 0;
};

// ----------------------------------------------------------------------

#endif // WJSCPP_SETTINGS_H