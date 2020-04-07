#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include <map>
#include <string>
#include <vector>
#include <fallen.h>
#include <wsjcpp_employees.h>
#include <cmd_handlers.h>

// ---------------------------------------------------------------------
// employ enum code results

enum EmployResult {
    OK,
    DATABASE_ERROR,
    ALREADY_EXISTS,
    GAME_NOT_FOUND,
    QUEST_NOT_FOUND,
    LEAK_NOT_FOUND,
    ERROR_NAME_IS_EMPTY,
};

// ----------------------------------------------------------------------

enum WsjcppSettingStorageType {
    WJSCPP_SETTING_IN_NONE,
    WJSCPP_SETTING_IN_FILE,
    WJSCPP_SETTING_IN_DATABASE,
    WJSCPP_SETTING_IN_RUNTIME
};

// ----------------------------------------------------------------------

enum WsjcppSettingDataType {
    WJSCPP_SETTING_TYPE_NONE,
    WJSCPP_SETTING_TYPE_STRING,
    WJSCPP_SETTING_TYPE_PASSWORD,
    WJSCPP_SETTING_TYPE_DIRPATH,
    WJSCPP_SETTING_TYPE_FILEPATH,
    WJSCPP_SETTING_TYPE_TEXT,
    WJSCPP_SETTING_TYPE_NUMBER,
    WJSCPP_SETTING_TYPE_BOOLEAN,
    WJSCPP_SETTING_TYPE_LIST,
    WJSCPP_SETTING_TYPE_JSON
};

// ----------------------------------------------------------------------

class WsjcppSettingItem {
    public:
        WsjcppSettingItem(const std::string &sSettingGroup, const std::string &sSettingName);
        // WsjcppSettingItem(const WsjcppSettingItem &item);
        WsjcppSettingItem &inFile();
        WsjcppSettingItem &inDatabase();
        WsjcppSettingItem &inRuntime();
        WsjcppSettingItem &readonly();
        WsjcppSettingItem &string(const std::string &sDefaultStringValue);
        WsjcppSettingItem &password(const std::string &sDefaultPasswordValue);
        WsjcppSettingItem &dirPath(const std::string &sDefaultDirPathValue);
        WsjcppSettingItem &filePath(const std::string &sDefaultFilePathValue);
        WsjcppSettingItem &text(const std::string &sDefaultTextValue);
        WsjcppSettingItem &number(int nDefaultNumberValue);
        WsjcppSettingItem &boolean(bool bDefaultBooleanValue);
        WsjcppSettingItem &json();
        WsjcppSettingItem &list();
        
        void checkWithThrow() const;
        bool isInited() const;
        bool isReadonly() const;
        std::string getName() const;
        std::string getGroupName() const;
        bool isFromFile() const;
        bool isFromDatabase() const;
        bool isFromRuntime() const;

        bool isString() const;
        bool isPassword() const;
        bool isDirPath() const;
        bool isFilePath() const;
        bool isText() const;
        bool isNumber() const;
        bool isBoolean() const;
        bool isJson() const;
        bool isList() const;
        
        bool isLikeString() const;

        std::string getDefaultStringValue() const;
        std::string getStringValue() const;
        void setStringValue(const std::string &sStringValue);

        std::string getDefaultPasswordValue() const;
        std::string getPasswordValue() const;
        void setPasswordValue(const std::string &sPasswordValue);

        std::string getDefaultDirPathValue() const;
        std::string getDirPathValue() const;
        void setDirPathValue(const std::string &sDirPathValue);

        std::string getDefaultFilePathValue() const;
        std::string getFilePathValue() const;
        void setFilePathValue(const std::string &sFilePathValue);

        std::string getDefaultTextValue() const;
        std::string getTextValue() const;
        void setTextValue(const std::string &sTextValue);

        int getDefaultNumberValue() const;
        int getNumberValue() const;
        void setNumberValue(int nNumberValue);

        bool getDefaultBooleanValue() const;
        bool getBooleanValue() const;
        void setBooleanValue(bool bBooleanValue);
        
        nlohmann::json toJson(bool bHidePassword);
        std::string convertValueToString(bool bHidePassword) const;
        std::string convertTypeToString() const;

    private:
        std::string TAG;
        std::string m_sSettingName;
        std::string m_sSettingGroup;
        bool m_bReadonly;
        WsjcppSettingStorageType m_nStorageType;
        std::string m_sStorageType;
        WsjcppSettingDataType m_nSettingType;
        std::string m_sSettingType;
        bool m_bInited;

        // isString
        std::string m_sDefaultStringValue;
        std::string m_sStringValue;

        // isPassword
        std::string m_sDefaultPasswordValue;
        std::string m_sPasswordValue;

        // isDirPath
        std::string m_sDefaultDirPathValue;
        std::string m_sDirPathValue;

        // isFilePath
        std::string m_sDefaultFilePathValue;
        std::string m_sFilePathValue;

        // isText
        std::string m_sDefaultTextValue;
        std::string m_sTextValue;

        // isNumber
        int m_nDefaultNumberValue;
        int m_nNumberValue;

        // isBoolean
        bool m_bDefaultBooleanValue;
        bool m_bBooleanValue;
};

// ----------------------------------------------------------------------

class WsjcppSettingListener {
    public:
        virtual void onSettingsChanged(const WsjcppSettingItem *pSettingItem) = 0;
};

// ----------------------------------------------------------------------

class WsjcppSettingsStore {
    public:
        virtual std::map<std::string, std::string> loadAllSettings() = 0;
        virtual void updateSettingItem(const WsjcppSettingItem *pSettingItem) = 0;
        virtual void initSettingItem(WsjcppSettingItem *pSettingItem) = 0;
};

// ---------------------------------------------------------------------
// WsjcppEmployGlobalSettings

class EmployGlobalSettings : public WsjcppEmployBase {
    public:
        EmployGlobalSettings();
        static std::string name() { return "EmployGlobalSettings"; }
        virtual bool init(); // here will be init from file
        virtual bool deinit();
        void setWorkDir(const std::string &sWorkDir); // TODO deprecated

        WsjcppSettingItem &registrySetting(const std::string &sSettingGroup, const std::string &sSettingName);

        const WsjcppSettingItem &get(const std::string &sSettingName);
        bool exists(const std::string &sSettingName);
        void update(const std::string &sSettingName, const std::string &sValue);
        void update(const std::string &sSettingName, int nValue);
        void update(const std::string &sSettingName, bool bValue);

        void addListener(WsjcppSettingListener *);
        void removeListener(WsjcppSettingListener *);
        bool initFromDatabase(WsjcppSettingsStore *pDatabaseSettingsStore);
        std::string getFilepathConf() const;
        nlohmann::json toJson(bool bHidePassword);
        void printSettings() const;

    private:
        std::string TAG;
        std::string m_sWorkDir;
        std::string m_sFilepathConf;
        WsjcppSettingsStore *m_pDatabaseSettingsStore;

        bool findFileConfig();
        bool initFromFile();
        void eventSettingsChanged(const WsjcppSettingItem *pSettingItem);
        std::map <std::string, WsjcppSettingItem *> m_mapSettingItems;
        std::vector<WsjcppSettingListener *> m_vListeners;
};

// ---------------------------------------------------------------------
// WJSCppEmployServer

class EmployServer : public WsjcppEmployBase {
    public:
        EmployServer();
        static std::string name() { return "EmployServer"; }
        virtual bool init();
        virtual bool deinit();
        bool validateInputParameters(WsjcppError &error, CmdHandlerBase *pCmdHandler, const nlohmann::json& jsonMessage);
        void setServer(IWebSocketServer *pWebSocketServer);
        void sendToAll(const nlohmann::json& jsonMessage);
        void sendToOne(QWebSocket *pClient, const nlohmann::json& jsonMessage);

    private:
        std::string TAG;
        IWebSocketServer *m_pWebSocketServer;
};

#endif // EMPLOYEES_H
