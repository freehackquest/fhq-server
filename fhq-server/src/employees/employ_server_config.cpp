#include <employ_server_config.h>
#include <sys/stat.h>
#include <storages.h>
#include <fallen.h>
#include <fstream>
#include <regex>        // regex, sregex_token_iterator
#include <string>
#include <sstream>
#include <iostream>

REGISTRY_WJSCPP_EMPLOY(EmployServerConfig)

// ---------------------------------------------------------------------

EmployServerConfig::EmployServerConfig()
    : EmployBase(EmployServerConfig::name(), {}) {
    
    TAG = EmployServerConfig::name();

    m_mapDefaultOptions["storage_type"] = "mysql";
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
    m_bDatabase_usemysql = true;

    // sql
    m_sStorageType = "mysql"; // default
    m_bDatabase_usemysql = true;
    m_sDatabase_host = "localhost";
    m_nDatabase_port = 3306;
    m_sDatabase_name = "freehackquest";
    m_sDatabase_user = "freehackquest_u";
    m_sDatabase_password = "freehackquest_p";

    // local nosql
    m_sDatabase_path = "/var/lib/fhq-server/data";

    m_nServer_port = 1234;
    m_bServer_ssl_on = false;
    m_nServer_ssl_port = 4613;
    m_sServer_ssl_key_file = "/etc/ssl/private/localhost.key";
    m_sServer_ssl_cert_file = "/etc/ssl/certs/localhost.pem";

    // web - default options
    m_nWeb_port = 7080;
    m_nWeb_max_threads = 4;
    m_sWeb_admin_folder = "/usr/share/fhq-server/web-admin";
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

    m_sStorageType = parseConfig.stringValue("storage_type", m_sStorageType);
    // TODO check support
    if (!Storages::support(m_sStorageType)) {
        Log::err(TAG, "Not support storage " + m_sStorageType);
        return false;
    }

    m_bDatabase_usemysql = parseConfig.boolValue("usemysql", m_bDatabase_usemysql);
    if (m_bDatabase_usemysql) {
        // Deprecated
        m_sDatabase_host = parseConfig.stringValue("dbhost", m_sDatabase_host);
        m_nDatabase_port = parseConfig.intValue("dbport", m_nDatabase_port);
        m_sDatabase_name = parseConfig.stringValue("dbname", m_sDatabase_name);
        m_sDatabase_user = parseConfig.stringValue("dbuser", m_sDatabase_user);
        m_sDatabase_password = parseConfig.stringValue("dbpass", m_sDatabase_password);

        Log::info(TAG, "Database host: " + m_sDatabase_host);
        Log::info(TAG, "Database port: " + std::to_string(m_nDatabase_port));
        Log::info(TAG, "Database name: " + m_sDatabase_name);
        Log::info(TAG, "Database user: " + m_sDatabase_user);
    } else {
        m_sDatabase_path = parseConfig.stringValue("dbpath", m_sDatabase_path);
        Log::info(TAG, "Database: using " + m_sDatabase_path);
    }

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
    return true;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::filepathConf() {
    return m_sFilepathConf;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::storageType() {
    return m_sStorageType;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databaseHost() {
    return m_sDatabase_host;
}

// ---------------------------------------------------------------------

int EmployServerConfig::databasePort() {
    return m_nDatabase_port;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databaseName() {
    return m_sDatabase_name;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databaseUser() {
    return m_sDatabase_user;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databasePassword() {
    return m_sDatabase_password;
}

// ---------------------------------------------------------------------

bool EmployServerConfig::databaseUseMySQL() {
    return m_bDatabase_usemysql;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databasePath() {
    return m_sDatabase_path;
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
