#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include <map>
#include <string>
#include <vector>
#include <fallen.h>
#include <wsjcpp_settings.h>
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
        Log::throw_err(TAG, "Not found employ " + sEmployName);
    }
    T *pTEmploy = dynamic_cast<T*>(pEmploy);
    if (pTEmploy == NULL) {
        Log::throw_err(TAG, "Employ could not cast to T [" + sEmployName + "]");
    }
    return pTEmploy;
}

// ---------------------------------------------------------------------
// WJSCppEmployConfig

class EmployGlobalSettings : public WSJCppEmployBase {
    public:
        EmployGlobalSettings();
        static std::string name() { return "EmployGlobalSettings"; }
        virtual bool init(); // here will be init from file
        void setWorkDir(const std::string &sWorkDir);

        void regestryItem(const WSJCppSettingItem &item);
        WSJCppSettingItem &regestrySetting(const std::string &sSettingName);

        const WSJCppSettingItem &get(const std::string &sSettingName);
        void update(const WSJCppSettingItem &item);
        void addListener(WSJCppSettingListener *);
        bool initFromDatabase();

    private:
        std::string TAG;
        std::string m_sWorkDir;
        std::string m_sFilepathConf;
        WSJCppSettingItem *m_pSettingNone;
        bool findFileConfig();
        bool initFromFile();
        std::map <std::string, WSJCppSettingItem *> m_mapSettingItems;
        std::vector<WSJCppSettingListener *> m_vListeners;
};

// ---------------------------------------------------------------------
// WJSCppEmployConfig

class EmployServerConfig : public WSJCppEmployBase {
    public:
        EmployServerConfig();
        static std::string name() { return "EmployServerConfig"; }
        virtual bool init();
        
        // configs
        std::string filepathConf();

        bool serverSslOn();
        int serverPort();
        int serverSslPort();
        std::string serverSslKeyFile();
        std::string serverSslCertFile();
        
        void setWorkDir(const std::string &sWorkDir);

        int webPort();
        int webMaxThreads();
        std::string webAdminFolder();
        std::string webUserFolder();

    private:
        std::string TAG;
        std::string m_sWorkDir;
        std::map <std::string, std::string> m_mapDefaultOptions;
    
        std::string m_sFilepathConf;
        
        // server settings
        bool m_bServer_ssl_on;
        int m_nServer_port;
        int m_nServer_ssl_port;
        std::string m_sServer_ssl_key_file;
        std::string m_sServer_ssl_cert_file;

        // web config
        int m_nWeb_port;
        int m_nWeb_max_threads;
        std::string m_sWeb_admin_folder;
        std::string m_sWeb_user_folder;
};

// ---------------------------------------------------------------------
// WJSCppEmployConfig

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
