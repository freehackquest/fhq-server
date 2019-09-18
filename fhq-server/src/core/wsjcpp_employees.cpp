#include <fallen.h>
#include <wsjcpp_employees.h>
#include <employ_server_info.h>
#include <employ_settings.h>
#include <algorithm>
#include <storages.h>

// ---------------------------------------------------------------------

std::map<std::string, WSJCppEmployBase*> *g_pEmployees = NULL;
std::vector<std::string> *g_pInitEmployees = NULL;

// ---------------------------------------------------------------------

void Employees::initGlobalVariables() {
    if (g_pEmployees == NULL) {
        // Log::info(std::string(), "Create employees map");
        g_pEmployees = new std::map<std::string, WSJCppEmployBase*>();
    }
    if (g_pInitEmployees == NULL) {
        // Log::info(std::string(), "Create init employees vector");
        g_pInitEmployees = new std::vector<std::string>();
    }
}

// ---------------------------------------------------------------------

void Employees::addEmploy(const std::string &sName, WSJCppEmployBase* pEmploy) {
    Employees::initGlobalVariables();
    if (g_pEmployees->count(sName)) {
        Log::err(sName, "Already registered");
    } else {
        g_pEmployees->insert(std::pair<std::string, WSJCppEmployBase*>(sName,pEmploy));
        // Log::info(sName, "Registered");
    }
}

// ---------------------------------------------------------------------

bool Employees::init(const std::vector<std::string> &vStart) {
    Employees::initGlobalVariables();

    for (unsigned int i = 0; i < vStart.size(); i++) {
        g_pInitEmployees->push_back(vStart[i]);
    }

    std::string TAG = "Employees_init";
    bool bRepeat = true;
    while (bRepeat) {
        bRepeat = false;
        std::map<std::string, WSJCppEmployBase*>::iterator it = g_pEmployees->begin();
        for (; it!=g_pEmployees->end(); ++it) {
            std::string sEmployName = it->first;
            WSJCppEmployBase *pEmploy = it->second;

            if (std::find(g_pInitEmployees->begin(), g_pInitEmployees->end(), sEmployName) != g_pInitEmployees->end()) {
                continue;
            }

            unsigned int nRequireLoaded = 0;
            for (unsigned int i = 0; i < pEmploy->loadAfter().size(); i++) {
                std::string sRequireEmploy = pEmploy->loadAfter()[i];
                if (std::find(g_pInitEmployees->begin(), g_pInitEmployees->end(), sRequireEmploy) != g_pInitEmployees->end()) {
                    nRequireLoaded++;
                }
            }
            if (pEmploy->loadAfter().size() == nRequireLoaded) {
                if (!pEmploy->init()) {
                    Log::err(TAG, "Init " + sEmployName + " ... FAIL");
                    return false;
                }
                g_pInitEmployees->push_back(sEmployName);
                bRepeat = true;
                Log::info(TAG, "Init " + sEmployName + " ... OK");
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

// WSJCppEmployBase
WSJCppEmployBase::WSJCppEmployBase(const std::string &sName, const std::vector<std::string> &vAfter) {
    TAG = sName;
    m_sName = sName;

    for (unsigned int i = 0; i < vAfter.size(); i++) {
        m_vLoadAfter.push_back(vAfter[i]);
    }
    Employees::addEmploy(m_sName, this);
}

// ---------------------------------------------------------------------

const std::vector<std::string> &WSJCppEmployBase::loadAfter() {
    return m_vLoadAfter;
}

// ----------------------------------------------------------------------
// WSJCppSettingItem

WSJCppSettingItem::WSJCppSettingItem(const std::string &sSettingGroup, const std::string &sSettingName) {
    TAG = "WSJCppSettingItem-" + sSettingName;
    m_sSettingName = sSettingName;
    m_sSettingGroup = sSettingGroup;
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

// ---------------------------------------------------------------------
// EmployGlobalSettings

REGISTRY_WJSCPP_EMPLOY(EmployGlobalSettings)

// ---------------------------------------------------------------------

EmployGlobalSettings::EmployGlobalSettings()
    : WSJCppEmployBase(EmployGlobalSettings::name(), {}) {

    TAG = EmployGlobalSettings::name();

    // basicly
    this->regestrySetting("runtime", "work_dir").string("").inRuntime().readonly();
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::EmployGlobalSettings::init() {
    // checking settings
    std::map<std::string, WSJCppSettingItem*>::iterator it;
    for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
        WSJCppSettingItem *pItem = it->second;
        pItem->checkWithThrow();
    }
    return this->initFromFile();
}

// ---------------------------------------------------------------------

// TODO redesign to ->update
void EmployGlobalSettings::setWorkDir(const std::string &sWorkDir) {
    m_sWorkDir = sWorkDir;
}

// ---------------------------------------------------------------------

WSJCppSettingItem &EmployGlobalSettings::regestrySetting(const std::string &sSettingGroup, const std::string &sSettingName) {
    std::map<std::string, WSJCppSettingItem*>::iterator it;
    it = m_mapSettingItems.find(sSettingName);
    if (it != m_mapSettingItems.end()) {
        Log::throw_err(TAG, "Setting '" + sSettingName + "' already registered");
    }
    WSJCppSettingItem *pItem = new WSJCppSettingItem(sSettingGroup, sSettingName);
    m_mapSettingItems.insert(std::pair<std::string, WSJCppSettingItem*>(sSettingName, pItem));
    return *pItem;
}

// ---------------------------------------------------------------------

const WSJCppSettingItem &EmployGlobalSettings::get(const std::string &sSettingName) {
    std::map<std::string, WSJCppSettingItem*>::iterator it;
    it = m_mapSettingItems.find(sSettingName);
    if (it == m_mapSettingItems.end()) {
        Log::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
    }
    return *(it->second);
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::update(const WSJCppSettingItem &item) {
    // TODO
    Log::throw_err(TAG, "Not implemented yet");
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::initFromDatabase() {
    // TODO
    Log::throw_err(TAG, "Not implemented yet");
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::findFileConfig() {
    struct PossibleFileConfigs {
        PossibleFileConfigs(const std::string &sDirPath, const std::string &sFilePathConf) :
            sDirPath(sDirPath), sFilePathConf(sFilePathConf) {

        };
        std::string sDirPath;
        std::string sFilePathConf;
    };

    std::vector<PossibleFileConfigs> vSearchConfigFile;

    if (m_sWorkDir != "") {
        // TODO convert to fullpath
        vSearchConfigFile.push_back(PossibleFileConfigs(m_sWorkDir + "/conf.d/", m_sWorkDir + "/conf.d/fhq-server.conf"));
    } else {
        // TODO convert to fullpath
        vSearchConfigFile.push_back(PossibleFileConfigs("/etc/fhq-server/", "/etc/fhq-server/fhq-server.conf"));
    }

    for (int i = 0; i < vSearchConfigFile.size(); i++) {
        PossibleFileConfigs tmp = vSearchConfigFile[i];
        if (Fallen::fileExists(tmp.sFilePathConf)) {
            m_sFilepathConf = tmp.sFilePathConf;
            m_sWorkDir = tmp.sDirPath;
            Log::info(TAG, "Found config file " + tmp.sFilePathConf);
            break;
        } else {
            Log::warn(TAG, "Not found possible config file " + tmp.sFilePathConf);
        }
    }
    
    if (m_sFilepathConf == "") {
        Log::err(TAG, "Not found config file");
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::initFromFile() {
    if (!this->findFileConfig()) {
        return false;
    }

    WJSCppParseConfig parseConfig(m_sFilepathConf);
    parseConfig.load();

    std::map<std::string, WSJCppSettingItem*>::iterator it;
    for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
        WSJCppSettingItem *pItem = it->second;
        if (pItem->isFromFile()) {
            if (pItem->isString()) {
                std::string s = parseConfig.stringValue(pItem->getName(), pItem->getDefaultStringValue());
                pItem->setStringValue(s);
                // TODO run validators
            } else if (pItem->isNumber()) {
                int nValue = parseConfig.intValue(pItem->getName(), pItem->getDefaultNumberValue());
                pItem->setNumberValue(nValue);
                // TODO run validators
            } else if (pItem->isBoolean()) {
                bool bValue = parseConfig.boolValue(pItem->getName(), pItem->getDefaultBooleanValue());
                pItem->setBooleanValue(bValue);
                // TODO run validators
            } else {
                Log::throw_err(TAG, "Not implemented yet");
            }
        }
    }

    // missing settings in file
    for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
        WSJCppSettingItem *pItem = it->second;
        if (pItem->isFromFile() && !pItem->isInited()) {
            Log::warn(TAG, "Missing setting: '" + pItem->getName() + "' in file '" + m_sFilepathConf + "'");
        }
    }

    // excess settings in file
    std::map<std::string, std::string> mapValues = parseConfig.getValues();
    std::map<std::string, std::string>::iterator itV;
    for (itV = mapValues.begin(); itV != mapValues.end(); ++itV) {
        std::string sSettingName = itV->first;
        it = m_mapSettingItems.find(sSettingName);
        if (it == m_mapSettingItems.end()) {
            Log::warn(TAG, "Excess setting: '" + sSettingName + "' in file '" + m_sFilepathConf + "'");
        }
    }
    return true;
}

// ---------------------------------------------------------------------
// EmployServerConfig

REGISTRY_WJSCPP_EMPLOY(EmployServerConfig)

// ---------------------------------------------------------------------

EmployServerConfig::EmployServerConfig()
    : WSJCppEmployBase(EmployServerConfig::name(), { EmployGlobalSettings::name() }) {
    
    TAG = EmployServerConfig::name();

    m_mapDefaultOptions["port"] = "1234";
    m_mapDefaultOptions["ssl_on"] = "no";
    m_mapDefaultOptions["ssl_port"] = "4613";
    m_mapDefaultOptions["ssl_key_file"] = "/etc/ssl/private/localhost.key";
    m_mapDefaultOptions["ssl_cert_file"] = "/etc/ssl/certs/localhost.crt";
    m_mapDefaultOptions["web_port"] = "7080";
    m_mapDefaultOptions["web_max_threads"] = "4";
    m_mapDefaultOptions["web_admin_folder"] = "/usr/share/fhq-server/web-admin";

    // default settings
    m_bServer_ssl_on = false;

    m_nServer_port = 1234;
    m_bServer_ssl_on = false;
    m_nServer_ssl_port = 4613;
    m_sServer_ssl_key_file = "/etc/ssl/private/localhost.key";
    m_sServer_ssl_cert_file = "/etc/ssl/certs/localhost.pem";

    // web - default options
    m_nWeb_port = 7080;
    m_nWeb_max_threads = 4;
    m_sWeb_admin_folder = "/usr/share/fhq-server/web-admin";
    m_sWeb_user_folder = "/usr/share/fhq-server/fhq-web-user";
}

// ---------------------------------------------------------------------

void EmployServerConfig::setWorkDir(const std::string &sWorkDir) {
    m_sWorkDir = sWorkDir;
}

// ---------------------------------------------------------------------

bool EmployServerConfig::init() {
    // TODO: redesign find folder with configs
    struct PossibleFileConfigs {
        PossibleFileConfigs(const std::string &sDirPath, const std::string &sFilePathConf) :
            sDirPath(sDirPath), sFilePathConf(sFilePathConf) {

        };
        std::string sDirPath;
        std::string sFilePathConf;
    };

    std::vector<PossibleFileConfigs> vSearchConfigFile;

    if (m_sWorkDir != "") {
        // TODO convert to fullpath
        vSearchConfigFile.push_back(PossibleFileConfigs(m_sWorkDir + "/conf.d/", m_sWorkDir + "/conf.d/fhq-server.conf"));
    } else {
        // TODO convert to fullpath
        vSearchConfigFile.push_back(PossibleFileConfigs("./", "fhq-server.conf"));
        // vSearchConfigFile.push_back("/etc/freehackquest-backend/conf.ini");
        // vSearchConfigFile.push_back("/etc/fhq-server/conf.ini");
        // vSearchConfigFile.push_back("etc/freehackquest-backend/conf.ini");
        vSearchConfigFile.push_back(PossibleFileConfigs("/etc/fhq-server/", "/etc/fhq-server/fhq-server.conf"));
    }

    for (int i = 0; i < vSearchConfigFile.size(); i++) {
        PossibleFileConfigs tmp = vSearchConfigFile[i];
        if (Fallen::fileExists(tmp.sFilePathConf)) {
            m_sFilepathConf = tmp.sFilePathConf;
            m_sWorkDir = tmp.sDirPath;
            Log::info(TAG, "Found config file " + tmp.sFilePathConf);
            break;
        } else {
            Log::warn(TAG, "Not found possible config file " + tmp.sFilePathConf);
        }
    }
    
    if (m_sFilepathConf == "") {
        Log::err(TAG, "Not found config file");
        return false;
    }

    WJSCppParseConfig parseConfig(m_sFilepathConf);
    parseConfig.load();

    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();

    m_nServer_port = parseConfig.intValue("port", m_nServer_port);
    m_bServer_ssl_on = parseConfig.boolValue("ssl_on", m_bServer_ssl_on);
    if (m_bServer_ssl_on) {
        m_nServer_ssl_port = parseConfig.intValue("ssl_port", m_nServer_ssl_port);
        m_sServer_ssl_key_file = parseConfig.stringValue("ssl_key_file", m_sServer_ssl_key_file);
        m_sServer_ssl_cert_file = parseConfig.stringValue("ssl_cert_file", m_sServer_ssl_cert_file);
    }

    m_nWeb_port = parseConfig.intValue("web_port", m_nWeb_port);
    m_nWeb_max_threads = parseConfig.intValue("web_max_threads", m_nWeb_max_threads);
    if (m_nWeb_max_threads <= 0) {
        Log::err(TAG, "Wrong option 'web_max_threads', values must be more then 0");
        return false;
    }

    if (m_nWeb_max_threads > 100) {
        Log::err(TAG, "Wrong option 'web_max_threads', values must be less then 0");
        return false;
    }

    m_sWeb_admin_folder = parseConfig.stringValue("web_admin_folder", m_sWeb_admin_folder);
    if (m_sWeb_admin_folder.length() > 0 && m_sWeb_admin_folder[0] != '/') {
        m_sWeb_admin_folder = m_sWorkDir + "/" + m_sWeb_admin_folder;
    }
    if (!Fallen::dirExists(m_sWeb_admin_folder)) {
        Log::err(TAG, "Wrong option 'web_admin_folder', because folder '" + m_sWeb_admin_folder + "' does not exists");
        return false;
    } else {
        Log::info(TAG, "Web: web_admin_folder " + m_sWeb_admin_folder);
    }

    m_sWeb_user_folder = parseConfig.stringValue("web_user_folder", m_sWeb_user_folder);
    if (m_sWeb_user_folder.length() > 0 && m_sWeb_user_folder[0] != '/') {
        m_sWeb_user_folder = m_sWorkDir + "/" + m_sWeb_user_folder;
    }
    if (!Fallen::dirExists(m_sWeb_user_folder)) {
        Log::err(TAG, "Wrong option 'web_user_folder', because folder '" + m_sWeb_user_folder + "' does not exists");
        return false;
    } else {
        Log::info(TAG, "Web: web_user_folder " + m_sWeb_user_folder);
    }
    return true;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::filepathConf() {
    return m_sFilepathConf;
}

// ---------------------------------------------------------------------

bool EmployServerConfig::serverSslOn() {
    return m_bServer_ssl_on;
}

// ---------------------------------------------------------------------

int EmployServerConfig::serverPort() {
    return m_nServer_port;
}

// ---------------------------------------------------------------------

int EmployServerConfig::serverSslPort() {
    return m_nServer_ssl_port;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::serverSslKeyFile() {
    return m_sServer_ssl_key_file;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::serverSslCertFile() {
    return m_sServer_ssl_cert_file;
}

// ---------------------------------------------------------------------

int EmployServerConfig::webPort() {
    return m_nWeb_port;
}

// ---------------------------------------------------------------------

int EmployServerConfig::webMaxThreads() {
    return m_nWeb_max_threads;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::webAdminFolder() {
    return m_sWeb_admin_folder;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::webUserFolder() {
    return m_sWeb_user_folder;
}

// ---------------------------------------------------------------------

REGISTRY_WJSCPP_EMPLOY(EmployServer)

EmployServer::EmployServer()
    : WSJCppEmployBase(EmployServer::name(), {"start_server", EmployServerConfig::name(), EmployServerInfo::name()}) {
    TAG = EmployServer::name();
    m_pWebSocketServer = NULL;
}

// ---------------------------------------------------------------------

bool EmployServer::init() {
    return true;
}

// ---------------------------------------------------------------------

void EmployServer::setServer(IWebSocketServer *pWebSocketServer) {
    m_pWebSocketServer = pWebSocketServer;
}

// ---------------------------------------------------------------------

void EmployServer::sendToAll(const nlohmann::json& jsonMessage) {
    m_pWebSocketServer->sendToAll(jsonMessage);
}

// ---------------------------------------------------------------------

void EmployServer::sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) {
    m_pWebSocketServer->sendToOne(pClient, jsonMessage);
}

// ---------------------------------------------------------------------

bool EmployServer::validateInputParameters(WSJCppError &error, CmdHandlerBase *pCmdHandler, const nlohmann::json &jsonMessage) {
    try {
        // TODO check extra params

        for (CmdInputDef inDef : pCmdHandler->inputs()) {
            
            auto itJsonParamName = jsonMessage.find(inDef.getName());
            const auto endJson = jsonMessage.end();
            if (inDef.isRequired() && itJsonParamName == endJson) {
                error = WSJCppError(400, "Parameter '" + inDef.getName() + "' expected");
                return false;
            }

            if (itJsonParamName != endJson) {
                if (inDef.isInteger()) {
                    if (!itJsonParamName->is_number()) {
                        error = WSJCppError(400, "Parameter '" + inDef.getName() + "' must be integer");
                        return false;
                    }

                    int val = *itJsonParamName;
                    if (inDef.isMinVal() && val < inDef.getMinVal()) {
                        error = WSJCppError(400, "Parameter '" + inDef.getName() + "' must be more then " + std::to_string(inDef.getMinVal()));
                        return false;
                    }
                    if (inDef.isMaxVal() && val > inDef.getMaxVal()) {
                        error = WSJCppError(400, "Parameter '" + inDef.getName() + "' must be less then " + std::to_string(inDef.getMaxVal()));
                        return false;
                    }
                }

                if (inDef.isString()) {
                    std::string sVal = itJsonParamName->get_ref<std::string const&>();
                    std::string sError;
                    const std::vector<ValidatorStringBase *> vValidators = inDef.listOfValidators();
                    for (int i = 0; i < vValidators.size(); i++) {
                        if (!vValidators[i]->isValid(sVal, sError)) {
                            error = WSJCppError(400, "Wrong param '" + inDef.getName() + "': " + sError);
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    } catch(std::exception const &e) {
        error = WSJCppError(500, "InternalServerError");
        Log::err(TAG, std::string("validateInputParameters, ") + e.what());
        return false;
    }
}
