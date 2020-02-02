#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include <map>
#include <string>
#include <vector>
#include <fallen.h>
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

// ---------------------------------------------------------------------
// base employ class

class WSJCppEmployBase {
public:
    WSJCppEmployBase(
        const std::string &sName,
        const std::vector<std::string> &vLoadAfter);
    virtual bool init() = 0;
    const std::vector<std::string> &loadAfter();

private:
    std::string TAG;
    std::string m_sName;
    std::vector<std::string> m_vLoadAfter;
};

// ---------------------------------------------------------------------
// public employees

extern std::map<std::string, WSJCppEmployBase*> *g_pEmployees;
extern std::vector<std::string> *g_pInitEmployees;

class Employees {
    public:
        static void initGlobalVariables();
        static void addEmploy(const std::string &sName, WSJCppEmployBase* pEmploy);
        static bool init(const std::vector<std::string> &vLoadAfter);
};

// ---------------------------------------------------------------------
// RegistryEmploy
#define REGISTRY_WJSCPP_EMPLOY( classname ) \
    static classname * pWJSCppRegistry ## classname = new classname(); \

// ---------------------------------------------------------------------
// findEmploy

template <class T> T* findEmploy() {
    Employees::initGlobalVariables();
    std::string TAG = "findEmploy";
    std::string sEmployName = T::name();
    WSJCppEmployBase *pEmploy = NULL;
    if (g_pEmployees->count(sEmployName)) {
        pEmploy = g_pEmployees->at(sEmployName);
    }
    if (pEmploy == NULL) {
        WSJCppLog::throw_err(TAG, "Not found employ " + sEmployName);
    }
    T *pTEmploy = dynamic_cast<T*>(pEmploy);
    if (pTEmploy == NULL) {
        WSJCppLog::throw_err(TAG, "Employ could not cast to T [" + sEmployName + "]");
    }
    return pTEmploy;
}

// ----------------------------------------------------------------------

enum WSJCppSettingStorageType {
    WJSCPP_SETTING_IN_NONE,
    WJSCPP_SETTING_IN_FILE,
    WJSCPP_SETTING_IN_DATABASE,
    WJSCPP_SETTING_IN_RUNTIME
};

// ----------------------------------------------------------------------

enum WSJCppSettingDataType {
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

class WSJCppSettingItem {
    public:
        WSJCppSettingItem(const std::string &sSettingGroup, const std::string &sSettingName);
        // WSJCppSettingItem(const WSJCppSettingItem &item);
        WSJCppSettingItem &inFile();
        WSJCppSettingItem &inDatabase();
        WSJCppSettingItem &inRuntime();
        WSJCppSettingItem &readonly();
        WSJCppSettingItem &string(const std::string &sDefaultStringValue);
        WSJCppSettingItem &password(const std::string &sDefaultPasswordValue);
        WSJCppSettingItem &dirPath(const std::string &sDefaultDirPathValue);
        WSJCppSettingItem &filePath(const std::string &sDefaultFilePathValue);
        WSJCppSettingItem &text(const std::string &sDefaultTextValue);
        WSJCppSettingItem &number(int nDefaultNumberValue);
        WSJCppSettingItem &boolean(bool bDefaultBooleanValue);
        WSJCppSettingItem &json();
        WSJCppSettingItem &list();
        
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
        WSJCppSettingStorageType m_nStorageType;
        std::string m_sStorageType;
        WSJCppSettingDataType m_nSettingType;
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

class WSJCppSettingListener {
    public:
        virtual void onSettingsChanged(const WSJCppSettingItem *pSettingItem) = 0;
};

// ----------------------------------------------------------------------

class WSJCppSettingsStore {
    public:
        virtual std::map<std::string, std::string> loadAllSettings() = 0;
        virtual void updateSettingItem(const WSJCppSettingItem *pSettingItem) = 0;
        virtual void initSettingItem(WSJCppSettingItem *pSettingItem) = 0;
};

// ---------------------------------------------------------------------
// WSJCppEmployGlobalSettings

class EmployGlobalSettings : public WSJCppEmployBase {
    public:
        EmployGlobalSettings();
        static std::string name() { return "EmployGlobalSettings"; }
        virtual bool init(); // here will be init from file
        void setWorkDir(const std::string &sWorkDir); // TODO deprecated

        WSJCppSettingItem &registrySetting(const std::string &sSettingGroup, const std::string &sSettingName);

        const WSJCppSettingItem &get(const std::string &sSettingName);
        bool exists(const std::string &sSettingName);
        void update(const std::string &sSettingName, const std::string &sValue);
        void update(const std::string &sSettingName, int nValue);
        void update(const std::string &sSettingName, bool bValue);

        void addListener(WSJCppSettingListener *);
        void removeListener(WSJCppSettingListener *);
        bool initFromDatabase(WSJCppSettingsStore *pDatabaseSettingsStore);
        std::string getFilepathConf() const;
        nlohmann::json toJson(bool bHidePassword);
        void printSettings() const;

    private:
        std::string TAG;
        std::string m_sWorkDir;
        std::string m_sFilepathConf;
        WSJCppSettingsStore *m_pDatabaseSettingsStore;

        bool findFileConfig();
        bool initFromFile();
        void eventSettingsChanged(const WSJCppSettingItem *pSettingItem);
        std::map <std::string, WSJCppSettingItem *> m_mapSettingItems;
        std::vector<WSJCppSettingListener *> m_vListeners;
};

// ---------------------------------------------------------------------
// WJSCppEmployServer

class EmployServer : public WSJCppEmployBase {
    public:
        EmployServer();
        static std::string name() { return "EmployServer"; }
        virtual bool init();
        bool validateInputParameters(WSJCppError &error, CmdHandlerBase *pCmdHandler, const nlohmann::json& jsonMessage);
        void setServer(IWebSocketServer *pWebSocketServer);
        void sendToAll(const nlohmann::json& jsonMessage);
        void sendToOne(QWebSocket *pClient, const nlohmann::json& jsonMessage);

    private:
        std::string TAG;
        IWebSocketServer *m_pWebSocketServer;
};

#endif // EMPLOYEES_H
