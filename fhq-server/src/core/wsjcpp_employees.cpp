#include <fallen.h>
#include <wsjcpp_employees.h>
#include <employ_server_info.h>
#include <algorithm>
#include <storages.h>
#include <wsjcpp_core.h>
#include <iostream>

// ---------------------------------------------------------------------

std::map<std::string, WSJCppEmployBase*> *g_pEmployees = NULL;
std::vector<std::string> *g_pInitEmployees = NULL;

// ---------------------------------------------------------------------

void Employees::initGlobalVariables() {
    if (g_pEmployees == NULL) {
        // WSJCppLog::info(std::string(), "Create employees map");
        g_pEmployees = new std::map<std::string, WSJCppEmployBase*>();
    }
    if (g_pInitEmployees == NULL) {
        // WSJCppLog::info(std::string(), "Create init employees vector");
        g_pInitEmployees = new std::vector<std::string>();
    }
}

// ---------------------------------------------------------------------

void Employees::addEmploy(const std::string &sName, WSJCppEmployBase* pEmploy) {
    Employees::initGlobalVariables();
    if (g_pEmployees->count(sName)) {
        WSJCppLog::err(sName, "Already registered");
    } else {
        g_pEmployees->insert(std::pair<std::string, WSJCppEmployBase*>(sName,pEmploy));
        // WSJCppLog::info(sName, "Registered");
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
                    WSJCppLog::err(TAG, "Init " + sEmployName + " ... FAIL");
                    return false;
                }
                g_pInitEmployees->push_back(sEmployName);
                bRepeat = true;
                WSJCppLog::ok(TAG, "Init " + sEmployName + " ... OK");
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
    TAG = "WSJCppSettingItem-(group:" + sSettingGroup + ")/get(\"" + sSettingName + "\")";
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

/*WSJCppSettingItem::WSJCppSettingItem(const WSJCppSettingItem &item) {
    m_sSettingName = item.getName();
    TAG = "WSJCppSettingItem-" + sSettingName;
    m_bReadonly = item.isReadonly();
}*/

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inFile() {
    if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
        WSJCppLog::throw_err(TAG, "inFile: already defined another storage type");
    }
    m_nStorageType = WJSCPP_SETTING_IN_FILE;
    m_sStorageType = "file";
    m_bReadonly = true; // if in a file then it will be readonly
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inDatabase() {
    if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
        WSJCppLog::throw_err(TAG, "inDatabase: already defined another storage type");
    }
    m_nStorageType = WJSCPP_SETTING_IN_DATABASE;
    m_sStorageType = "database";
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::inRuntime() {
    if (m_nStorageType != WJSCPP_SETTING_IN_NONE) {
        WSJCppLog::throw_err(TAG, "inRuntime: already defined another storage type");
    }

    m_nStorageType = WJSCPP_SETTING_IN_RUNTIME;
    m_sStorageType = "runtime";
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
        WSJCppLog::throw_err(TAG, "string: already defined another data type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_STRING;
    m_sSettingType = "string";
    m_sDefaultStringValue = sDefaultStringValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::password(const std::string &sDefaultPasswordValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "password: already defined another data type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_PASSWORD;
    m_sSettingType = "password";
    m_sDefaultPasswordValue = sDefaultPasswordValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::dirPath(const std::string &sDefaultDirPathValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "dirPath: already defined another data type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_DIRPATH;
    m_sSettingType = "dir_path";
    m_sDefaultDirPathValue = sDefaultDirPathValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::filePath(const std::string &sDefaultFilePathValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "filePath: already defined another data type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_FILEPATH;
    m_sSettingType = "file_path";
    m_sDefaultFilePathValue = sDefaultFilePathValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::text(const std::string &sDefaultTextValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "text: already defined another data type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_TEXT;
    m_sSettingType = "text";
    m_sDefaultTextValue = sDefaultTextValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::number(int nDefaultNumberValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "number: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_NUMBER;
    m_sSettingType = "number";
    m_nDefaultNumberValue = nDefaultNumberValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::boolean(bool bDefaultBooleanValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "boolean: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_BOOLEAN;
    m_sSettingType = "boolean";
    m_bDefaultBooleanValue = bDefaultBooleanValue;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::json() {
    WSJCppLog::throw_err(TAG, "json, Not implemented yet");

    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "json: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_JSON;
    m_sSettingType = "json";
    return *this;
}

// ----------------------------------------------------------------------

WSJCppSettingItem & WSJCppSettingItem::list() {
    WSJCppLog::throw_err(TAG, "list, Not implemented yet");

    if (m_nSettingType != WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "json: already defined another storage type");
    }
    m_nSettingType = WJSCPP_SETTING_TYPE_LIST;
    m_sSettingType = "list";
    return *this;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::checkWithThrow() const {
    if (m_nSettingType == WJSCPP_SETTING_TYPE_NONE) {
        WSJCppLog::throw_err(TAG, "check: settings type could be NONE");
    }

    if (m_nStorageType == WJSCPP_SETTING_IN_NONE) {
        WSJCppLog::throw_err(TAG, "check: storage type could not be IN_NONE");
    }
    
    if (m_sSettingName == "") {
        WSJCppLog::throw_err(TAG, "check: name could not be empty");
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

std::string WSJCppSettingItem::getGroupName() const {
    return m_sSettingGroup; 
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

bool WSJCppSettingItem::isPassword() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_PASSWORD;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isDirPath() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_DIRPATH;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::isFilePath() const {
    return m_nSettingType == WJSCPP_SETTING_TYPE_FILEPATH;
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

bool WSJCppSettingItem::isLikeString() const {
    return 
        m_nSettingType == WJSCPP_SETTING_TYPE_STRING
        || m_nSettingType == WJSCPP_SETTING_TYPE_PASSWORD
        || m_nSettingType == WJSCPP_SETTING_TYPE_DIRPATH
        || m_nSettingType == WJSCPP_SETTING_TYPE_FILEPATH
        || m_nSettingType == WJSCPP_SETTING_TYPE_TEXT;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getDefaultStringValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
        WSJCppLog::throw_err(TAG, "getDefaultStringValue: Expected type string");
    }
    return m_sDefaultStringValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getStringValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
        WSJCppLog::throw_err(TAG, "getStringValue: Expected type string");
    }
    return m_bInited ? m_sStringValue : m_sDefaultStringValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setStringValue(const std::string &sStringValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_STRING) {
        WSJCppLog::throw_err(TAG, "setStringValue: Expected type string");
    }
    if (m_bInited && m_bReadonly) {
        WSJCppLog::throw_err(TAG, "setStringValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_sStringValue = sStringValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getDefaultPasswordValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_PASSWORD) {
        WSJCppLog::throw_err(TAG, "getDefaultPasswordValue: Expected type password");
    }
    return m_sDefaultPasswordValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getPasswordValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_PASSWORD) {
        WSJCppLog::throw_err(TAG, "getPasswordValue: Expected type password");
    }
    return m_bInited ? m_sPasswordValue : m_sDefaultPasswordValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setPasswordValue(const std::string &sPasswordValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_PASSWORD) {
        WSJCppLog::throw_err(TAG, "setStringValue: Expected type password");
    }
    if (m_bInited && m_bReadonly) {
        WSJCppLog::throw_err(TAG, "setPasswordValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_sPasswordValue = sPasswordValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getDefaultDirPathValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_DIRPATH) {
        WSJCppLog::throw_err(TAG, "getDefaultDirPathValue: Expected type dirpath");
    }
    return m_sDefaultDirPathValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getDirPathValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_DIRPATH) {
        WSJCppLog::throw_err(TAG, "getDirPathValue: Expected type dirpath");
    }
    return m_bInited ? m_sDirPathValue : m_sDefaultDirPathValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setDirPathValue(const std::string &sDirPathValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_DIRPATH) {
        WSJCppLog::throw_err(TAG, "setDirPathValue: Expected type dirpath");
    }
    if (m_bInited && m_bReadonly) {
        WSJCppLog::throw_err(TAG, "setDirPathValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_sDirPathValue = sDirPathValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getDefaultFilePathValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_FILEPATH) {
        WSJCppLog::throw_err(TAG, "getDefaultFilePathValue: Expected type filepath");
    }
    return m_sDefaultFilePathValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getFilePathValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_FILEPATH) {
        WSJCppLog::throw_err(TAG, "getFilePathValue: Expected type filepath");
    }
    return m_bInited ? m_sFilePathValue : m_sDefaultFilePathValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setFilePathValue(const std::string &sFilePathValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_FILEPATH) {
        WSJCppLog::throw_err(TAG, "setFilePathValue: Expected type filepath");
    }
    if (m_bInited && m_bReadonly) {
        WSJCppLog::throw_err(TAG, "setFilePathValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_sFilePathValue = sFilePathValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getDefaultTextValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_TEXT) {
        WSJCppLog::throw_err(TAG, "getDefaultTextValue: Expected type text");
    }
    return m_sDefaultTextValue;
}

// ----------------------------------------------------------------------

std::string WSJCppSettingItem::getTextValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_TEXT) {
        WSJCppLog::throw_err(TAG, "getTextValue: Expected type text");
    }
    return m_bInited ? m_sTextValue : m_sDefaultTextValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setTextValue(const std::string &sTextValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_TEXT) {
        WSJCppLog::throw_err(TAG, "setTextValue: Expected type text");
    }
    if (m_bInited && m_bReadonly) {
        WSJCppLog::throw_err(TAG, "setTextValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_sTextValue = sTextValue;
}

// ----------------------------------------------------------------------

int WSJCppSettingItem::getDefaultNumberValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
        WSJCppLog::throw_err(TAG, "getDefaultNumberValue: Expected type number");
    }
    return m_nDefaultNumberValue;
}

// ----------------------------------------------------------------------

int WSJCppSettingItem::getNumberValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
        WSJCppLog::throw_err(TAG, "getNumberValue: Expected type number");
    }
    return m_bInited ? m_nNumberValue : m_nDefaultNumberValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setNumberValue(int nNumberValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_NUMBER) {
        WSJCppLog::throw_err(TAG, "setNumberValue: Expected type number");
    }

    if (m_bInited && m_bReadonly) {
        WSJCppLog::throw_err(TAG, "setNumberValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_nNumberValue = nNumberValue;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::getDefaultBooleanValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
        WSJCppLog::throw_err(TAG, "getBooleanValue: Expected type boolean");
    }
    return m_bDefaultBooleanValue;
}

// ----------------------------------------------------------------------

bool WSJCppSettingItem::getBooleanValue() const {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
        WSJCppLog::throw_err(TAG, "getBooleanValue: Expected type boolean");
    }
    return m_bInited ? m_bBooleanValue : m_bDefaultBooleanValue;
}

// ----------------------------------------------------------------------

void WSJCppSettingItem::setBooleanValue(bool bBooleanValue) {
    if (m_nSettingType != WJSCPP_SETTING_TYPE_BOOLEAN) {
        WSJCppLog::throw_err(TAG, "setBooleanValue: Expected type boolean");
    }

    if (m_bInited && m_bReadonly) {
        WSJCppLog::throw_err(TAG, "setBooleanValue: Could not change value for readonly setting");
    }
    m_bInited = true;
    m_bBooleanValue = bBooleanValue;
}

// ---------------------------------------------------------------------

nlohmann::json WSJCppSettingItem::toJson(bool bHidePassword) {
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
        jsonSett["value"] =  bHidePassword ? "******" : getPasswordValue();
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

std::string WSJCppSettingItem::convertValueToString(bool bHidePassword) const {
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
        WSJCppLog::throw_err(TAG, "convertValueToString, Not implemented type of setting");
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WSJCppSettingItem::convertTypeToString() const {
    return m_sSettingType;
}

// ---------------------------------------------------------------------
// EmployGlobalSettings

REGISTRY_WJSCPP_EMPLOY(EmployGlobalSettings)

// ---------------------------------------------------------------------

EmployGlobalSettings::EmployGlobalSettings()
    : WSJCppEmployBase(EmployGlobalSettings::name(), {}) {

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
    WSJCppLog::info(TAG, "EmployGlobalSettings");
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

WSJCppSettingItem &EmployGlobalSettings::registrySetting(const std::string &sSettingGroup, const std::string &sSettingName) {
    std::map<std::string, WSJCppSettingItem*>::iterator it;
    it = m_mapSettingItems.find(sSettingName);
    if (it != m_mapSettingItems.end()) {
        WSJCppLog::throw_err(TAG, "Setting '" + sSettingName + "' already registered");
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
        WSJCppLog::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
    }
    return *(it->second);
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::exists(const std::string &sSettingName) {
    std::map<std::string, WSJCppSettingItem*>::iterator it;
    it = m_mapSettingItems.find(sSettingName);
    return it != m_mapSettingItems.end();
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::update(const std::string &sSettingName, const std::string &sValue) {
    std::map<std::string, WSJCppSettingItem*>::iterator it;
    it = m_mapSettingItems.find(sSettingName);
    if (it == m_mapSettingItems.end()) {
        WSJCppLog::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
    }
    WSJCppSettingItem* pItem = it->second;
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
        WSJCppLog::throw_err(TAG, "Error on updating setting '" + sSettingName + "' ");
    }
    if (pItem->isFromDatabase()) {
        m_pDatabaseSettingsStore->updateSettingItem(pItem);
    }
    // TODO send to listeners
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::update(const std::string &sSettingName, int nValue) {
    std::map<std::string, WSJCppSettingItem*>::iterator it;
    it = m_mapSettingItems.find(sSettingName);
    if (it == m_mapSettingItems.end()) {
        WSJCppLog::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
    }
    WSJCppSettingItem* pItem = it->second;
    if (pItem->isNumber()) {
        pItem->setNumberValue(nValue);
    } else {
        WSJCppLog::throw_err(TAG, "Error on updating number setting '" + sSettingName + "' ");
    }
    if (pItem->isFromDatabase()) {
        m_pDatabaseSettingsStore->updateSettingItem(pItem);
    }
    // TODO send to listeners
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::update(const std::string &sSettingName, bool bValue) {
    std::map<std::string, WSJCppSettingItem*>::iterator it;
    it = m_mapSettingItems.find(sSettingName);
    if (it == m_mapSettingItems.end()) {
        WSJCppLog::throw_err(TAG, "Setting '" + sSettingName + "' - not found");
    }
    WSJCppSettingItem* pItem = it->second;
    if (pItem->isBoolean()) {
        pItem->setBooleanValue(bValue);
    } else {
        WSJCppLog::throw_err(TAG, "Error on updating boolean setting '" + sSettingName + "' ");
    }
    if (pItem->isFromDatabase()) {
        m_pDatabaseSettingsStore->updateSettingItem(pItem);
    }
    // TODO send to listeners
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::initFromDatabase(WSJCppSettingsStore *pDatabaseSettingsStore) {
    m_pDatabaseSettingsStore = pDatabaseSettingsStore;
    std::map<std::string, WSJCppSettingItem*>::iterator it;

    std::map<std::string, std::string> vMap = pDatabaseSettingsStore->loadAllSettings();
    std::map<std::string, std::string>::iterator itDb;
    for (itDb = vMap.begin(); itDb != vMap.end(); ++itDb) {
        std::string sName = itDb->first;
        std::string sValue = itDb->second;
        it = m_mapSettingItems.find(sName);
        if (it == m_mapSettingItems.end()) {
            WSJCppLog::warn(TAG, "Excess setting in database, name '" + sName + "'");
        } else {
            WSJCppSettingItem *pItem = it->second;
            if (pItem->isString()) {
                pItem->setStringValue(sValue);
                WSJCppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
            } else if (pItem->isPassword()) {
                pItem->setPasswordValue(sValue);
                WSJCppLog::info(TAG, "Applyed settings from database: " + sName + " = (hidden)");
            } else if (pItem->isBoolean()) {
                pItem->setBooleanValue(sValue == "yes");
                WSJCppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
            } else if (pItem->isDirPath()) {
                pItem->setDirPathValue(sValue);
                WSJCppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
                // TODO check folder ?
            } else if (pItem->isFilePath()) {
                pItem->setFilePathValue(sValue);
                WSJCppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
            } else if (pItem->isNumber()) {
                int nValue = std::stoi(sValue);
                pItem->setNumberValue(nValue);
                WSJCppLog::info(TAG, "Applyed settings from database: " + sName + " = " + std::to_string(nValue));
            } else if (pItem->isText()) {
                pItem->setTextValue(sValue);
                WSJCppLog::info(TAG, "Applyed settings from database: " + sName + " = " + sValue);
            } else {
                WSJCppLog::throw_err(TAG, "type of setting (from database) not match with delared, name '" + sName + "'");
            }
        }
    }

    // check what not init yet in database
    for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
        WSJCppSettingItem *pItem = it->second;
        if (pItem->isFromDatabase() && !pItem->isInited()) {
            m_pDatabaseSettingsStore->initSettingItem(pItem);
        }
    }
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::printSettings() const {
    std::map<std::string, WSJCppSettingItem*>::iterator it;

    for (m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
        std::string sName = it->first;
        WSJCppSettingItem *pItem = it->second;

        std::cout << " * [" << pItem->getName() << "] => [" << pItem->convertValueToString(true) << "]" << std::endl;
        // jsonSett["group"] = pServerSettHelper->group();
        // jsonSett["type"] = pServerSettHelper->type();
    }
}

// ---------------------------------------------------------------------

bool EmployGlobalSettings::findFileConfig() {

    struct PossibleFileConfigs {
        PossibleFileConfigs(const std::string &sDirPath, const std::string &sFilePathConf) {
            m_sDirPath = WSJCppCore::doNormalizePath(sDirPath);
            m_sFilePathConf = WSJCppCore::doNormalizePath(sFilePathConf);
        };
        std::string m_sDirPath;
        std::string m_sFilePathConf;
    };

    std::vector<std::string> vSearchConfigFile;

    if (m_sWorkDir != "") {
        // TODO convert to fullpath
        vSearchConfigFile.push_back(m_sWorkDir + "/conf.d/");
    } else {
        // TODO convert to fullpath
        vSearchConfigFile.push_back("/etc/fhq-server/");
    }

    for (int i = 0; i < vSearchConfigFile.size(); i++) {
        std::string sDirPath = vSearchConfigFile[i];
        sDirPath = WSJCppCore::doNormalizePath(sDirPath);
        std::string sFilePathConf = sDirPath + "/fhq-server.conf";
        sFilePathConf = WSJCppCore::doNormalizePath(sFilePathConf);
        if (WSJCppCore::fileExists(sFilePathConf)) {
            m_sFilepathConf = sFilePathConf;
            m_sWorkDir = sDirPath;
            WSJCppLog::info(TAG, "Found config file " + sFilePathConf);
            break;
        } else {
            WSJCppLog::warn(TAG, "Not found possible config file " + sFilePathConf);
        }
    }
    
    if (m_sFilepathConf == "") {
        WSJCppLog::err(TAG, "Not found config file");
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
                WSJCppLog::info(TAG, pItem->getName() + " = " + s);
                // TODO run validators
            } else if (pItem->isPassword()) {
                std::string sPassword = parseConfig.stringValue(pItem->getName(), pItem->getDefaultPasswordValue());
                pItem->setPasswordValue(sPassword);
                WSJCppLog::info(TAG, pItem->getName() + " = (hidden)");
            } else if (pItem->isDirPath()) {
                std::string sDirPath = parseConfig.stringValue(pItem->getName(), pItem->getDefaultDirPathValue());
                if (sDirPath.length() > 0 && sDirPath[0] != '/') {
                    sDirPath = m_sWorkDir + "/" + sDirPath;
                    sDirPath = WSJCppCore::doNormalizePath(sDirPath);
                }
                if (!WSJCppCore::dirExists(sDirPath)) {
                    WSJCppLog::throw_err(TAG, "Wrong settings '" + pItem->getName() + "', because folder '" + sDirPath + "' does not exists");
                }
                pItem->setDirPathValue(sDirPath);
                WSJCppLog::info(TAG, pItem->getName() + " = " + sDirPath);
                // TODO run validators
             } else if (pItem->isFilePath()) {
                std::string sFilePath = parseConfig.stringValue(pItem->getName(), pItem->getDefaultFilePathValue());
                if (sFilePath.length() > 0 && sFilePath[0] != '/') {
                    sFilePath = m_sWorkDir + "/" + sFilePath;
                    sFilePath = WSJCppCore::doNormalizePath(sFilePath);
                }
                if (!WSJCppCore::fileExists(sFilePath)) {
                    WSJCppLog::throw_err(TAG, "Wrong settings '" + pItem->getName() + "', because file '" + sFilePath + "' does not exists");
                }
                pItem->setFilePathValue(sFilePath);
                WSJCppLog::info(TAG, pItem->getName() + " = " + sFilePath);
                // TODO run validators
            } else if (pItem->isNumber()) {
                int nValue = parseConfig.intValue(pItem->getName(), pItem->getDefaultNumberValue());
                pItem->setNumberValue(nValue);
                WSJCppLog::info(TAG, pItem->getName() + " = " + std::to_string(nValue));
                // TODO run validators
            } else if (pItem->isBoolean()) {
                bool bValue = parseConfig.boolValue(pItem->getName(), pItem->getDefaultBooleanValue());
                pItem->setBooleanValue(bValue);
                WSJCppLog::info(TAG, pItem->getName() + " = " + (bValue ? "yes" : "no"));
                // TODO run validators
            } else {
                WSJCppLog::throw_err(TAG, "initFromFile, Not implemented yet");
            }
        }
    }

    // missing settings in file
    for (it = m_mapSettingItems.begin(); it != m_mapSettingItems.end(); ++it) {
        WSJCppSettingItem *pItem = it->second;
        if (pItem->isFromFile() && !pItem->isInited()) {
            WSJCppLog::warn(TAG, "Missing setting: '" + pItem->getName() + "' in file '" + m_sFilepathConf + "'");
        }
    }

    // excess settings in file
    std::map<std::string, std::string> mapValues = parseConfig.getValues();
    std::map<std::string, std::string>::iterator itV;
    for (itV = mapValues.begin(); itV != mapValues.end(); ++itV) {
        std::string sSettingName = itV->first;
        it = m_mapSettingItems.find(sSettingName);
        if (it == m_mapSettingItems.end()) {
            WSJCppLog::warn(TAG, "Excess setting: '" + sSettingName + "' in file '" + m_sFilepathConf + "'");
        }
    }
    return true;
}

// ---------------------------------------------------------------------

std::string EmployGlobalSettings::getFilepathConf() const {
    return m_sFilepathConf;
}

// ---------------------------------------------------------------------

nlohmann::json EmployGlobalSettings::toJson(bool bPasswordHide) {
    nlohmann::json jsonSettings = nlohmann::json::array();

    std::map<std::string, WSJCppSettingItem *>::iterator it = m_mapSettingItems.begin();
    for (; it!=m_mapSettingItems.end(); ++it) {
        std::string sName = it->first;
        WSJCppSettingItem *pItem = it->second;
        jsonSettings.push_back(pItem->toJson(bPasswordHide));
    }
    return jsonSettings;
}

// ---------------------------------------------------------------------

void EmployGlobalSettings::eventSettingsChanged(const WSJCppSettingItem *pSettingItem) {
    for (int i = 0; i < m_vListeners.size(); i++) {
        m_vListeners[i]->onSettingsChanged(pSettingItem);
    }
}

// ---------------------------------------------------------------------

REGISTRY_WJSCPP_EMPLOY(EmployServer)

EmployServer::EmployServer()
    : WSJCppEmployBase(EmployServer::name(), {"start_server", EmployGlobalSettings::name(), EmployServerInfo::name()}) {
    TAG = EmployServer::name();
    m_pWebSocketServer = NULL;

    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();
    pGlobalSettings->registrySetting("ws_server", "port").number(1234).inFile();
    pGlobalSettings->registrySetting("ws_server", "ssl_on").boolean(false).inFile();
    pGlobalSettings->registrySetting("ws_server", "ssl_port").number(4613).inFile();
    pGlobalSettings->registrySetting("ws_server", "ssl_key_file").filePath("/etc/ssl/private/localhost.key").inFile();
    pGlobalSettings->registrySetting("ws_server", "ssl_cert_file").filePath("/etc/ssl/certs/localhost.crt").inFile();
    pGlobalSettings->registrySetting("web_server", "web_port").number(7080).inFile();
    pGlobalSettings->registrySetting("web_server", "web_max_threads").number(4).inFile();
    // TODO validators

    pGlobalSettings->registrySetting("google_map", "google_map_api_key").string("some").inDatabase();

    
    std::string sGroupServerFolders = "server_folders";
    
    // deprecated
    pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_quests")
        .dirPath("/var/www/html/fhq/files/quests/").inDatabase();
    // deprecated
    pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_quests_url")
        .string("https://freehackquest.com/files/quests/").inDatabase();
    
    // deprecated
    pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_games")
        .dirPath("/var/www/html/fhq/public/games/").inDatabase();
    // deprecated
    pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_games_url")
        .string("https://freehackquest.com/public/games/").inDatabase();

    // deprecated
    pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_users")
        .dirPath("/var/www/html/fhq/files/quests/").inDatabase();
    // deprecated (moved to public)
    pGlobalSettings->registrySetting(sGroupServerFolders, "server_folder_users_url")
        .string("https://freehackquest.com/files/quests/").inDatabase();

    // TODO move to userprofiles
    pGlobalSettings->registrySetting("user_profile", "profile_change_nick").boolean(true).inDatabase();
}

// ---------------------------------------------------------------------

bool EmployServer::init() {
    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();

    // TODO move to validators
    int nWeb_max_threads = pGlobalSettings->get("web_max_threads").getNumberValue();
    if (nWeb_max_threads <= 0) {
        WSJCppLog::err(TAG, "Wrong option 'web_max_threads', values must be more then 0");
        return false;
    }
    if (nWeb_max_threads > 100) {
        WSJCppLog::err(TAG, "Wrong option 'web_max_threads', values must be less then 0");
        return false;
    }

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
                if (itJsonParamName->is_null()) {
                    error = WSJCppError(400, "Parameter '" + inDef.getName() + "' could not be null");
                    return false;
                }

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
                    const std::vector<WSJCppValidatorStringBase *> vValidators = inDef.listOfValidators();
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
        WSJCppLog::err(TAG, std::string("validateInputParameters, ") + e.what());
        return false;
    }
}
