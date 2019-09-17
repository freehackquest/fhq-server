#ifndef WJSCPP_SETTINGS_H
#define WJSCPP_SETTINGS_H

#include <string>
#include <vector>
#include <regex>

enum WSJCppSettingStorageType {
    WJSCPP_SETTING_IN_NONE,
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
        WSJCppSettingItem &string(const std::string &sDefaultStringValue);
        WSJCppSettingItem &text(const std::string &sDefaultTextValue);
        WSJCppSettingItem &number(int nDefaultNumberValue);
        WSJCppSettingItem &boolean(bool bDefaultBooleanValue);
        WSJCppSettingItem &json();
        WSJCppSettingItem &list();
        
        void checkWithThrow() const;
        bool isInited() const;
        bool isReadonly() const;
        std::string getName() const;
        bool isFromFile() const;
        bool isFromDatabase() const;
        bool isFromRuntime() const;

        bool isString() const;
        bool isText() const;
        bool isNumber() const;
        bool isBoolean() const;
        bool isJson() const;
        bool isList() const;
        
        std::string getDefaultStringValue() const;
        std::string getStringValue() const;
        void setStringValue(const std::string &sStringValue);

        int getDefaultNumberValue() const;
        int getNumberValue() const;
        void setNumberValue(int nNumberValue);

        bool getDefaultBooleanValue() const;
        bool getBooleanValue() const;
        void setBooleanValue(bool bBooleanValue);

    private:
        std::string TAG;
        std::string m_sSettingName;
        bool m_bReadonly;
        WSJCppSettingStorageType m_nStorageType;
        WSJCppSettingDataType m_nSettingType;
        bool m_bInited;

        // isString
        std::string m_sDefaultStringValue;
        std::string m_sStringValue;

        // isNumber
        int m_nDefaultNumberValue;
        int m_nNumberValue;

        // isBoolean
        bool m_bDefaultBooleanValue;
        bool m_bBooleanValue;
};

class WSJCppSettingListener {
    public:
        virtual void eventSettingsChanged() = 0;
};

// ----------------------------------------------------------------------

#endif // WJSCPP_SETTINGS_H