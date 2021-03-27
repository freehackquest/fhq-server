#include <employ_orchestra.h>
#include <utils_lxd.h>

REGISTRY_WJSCPP_EMPLOY(EmployOrchestra)

// ---------------------------------------------------------------------

#include <fallen.h>
#include <algorithm>

#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <QStringList>

// ---------------------------------------------------------------------

EmployOrchestra::EmployOrchestra()
        : WsjcppEmployBase(EmployOrchestra::name(), { EmployGlobalSettings::name() }) {
    TAG = "EmployOrchestra";
    m_bTrusted = false;

    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

    std::string  sGroupLXD = "lxd";
    pGlobalSettings->registrySetting(sGroupLXD, "path_dir_lxc_ssl").string("/etc/fhq-server/lxd").inDatabase();
    pGlobalSettings->registrySetting(sGroupLXD, "lxd_server_ip").string("127.0.0.1").inDatabase();
    pGlobalSettings->registrySetting(sGroupLXD, "lxd_server_port").number(8443).inDatabase();
    pGlobalSettings->registrySetting(sGroupLXD, "lxd_mode").boolean(false).inDatabase();
}

// ---------------------------------------------------------------------

bool EmployOrchestra::init() {
    WsjcppLog::info(TAG, "Start init settings");
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    bool bLXDMode = pGlobalSettings->get("lxd_mode").getBooleanValue();
 
    if (!bLXDMode)
        return true;

    m_sPathDirLxcSSL = pGlobalSettings->get("path_dir_lxc_ssl").getStringValue();
    std::string lxd_server_ip = pGlobalSettings->get("lxd_server_ip").getStringValue();
    int nServerPort = pGlobalSettings->get("lxd_server_port").getNumberValue();
    std::string lxd_server_port = std::to_string(nServerPort);
    m_sLxdAddress = "https://" + lxd_server_ip + ":" + lxd_server_port;
    std::string sError;
    m_bTrusted = UtilsLXDAuth::check_trust_certs(sError);
    if (!m_bTrusted) {
        WsjcppLog::err(TAG,
                 "SSL certificates are not trusted.\nPlease configure the connection with the LXD. "
                 "Type ./fhq-server -mclxd");
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::initConnection() {

    WsjcppLog::info(TAG, "Start init connection");

    //Pull existing containers
    return !(m_bTrusted && !pull_container_names());
}

// ---------------------------------------------------------------------

std::string EmployOrchestra::lastError() {
    return m_sLastError;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::create_container(const std::string &sName, std::string &sError) {
    WsjcppLog::info(TAG, "Starting creation container " + sName);
    LXDContainer *const pContainer = new LXDContainer(sName);

    if (pContainer->create()) {
        WsjcppLog::info(TAG, "Created container " + sName);
    }

    if (!pContainer->get_error().empty()) {
        sError = pContainer->get_error();
        return false;
    }

    m_mapContainers.insert(std::pair<std::string, LXDContainer *>(sName, pContainer));
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::create_service(const ServiceConfig &serviceReq, std::string &sError) {
    ServiceLXD *const pService = new ServiceLXD(serviceReq);

    WsjcppLog::info(TAG, "Starting creation container " + serviceReq.name);

    if (!pService->create_container()) {
        sError = pService->get_error();
        WsjcppLog::err(TAG, "Failed created service. I can't create container.");
        return false;
    }

    m_mapServices.insert(std::pair<std::string, ServiceLXD *>(std::string(serviceReq.name), pService));
    return true;
}

bool EmployOrchestra::create_service(const nlohmann::json &jsonConfig, std::string &sError) {
    ServiceConfig config = ServiceConfig(jsonConfig);
    return create_service(config, sError);
}

// ---------------------------------------------------------------------

bool EmployOrchestra::find_container(const std::string &name, LXDContainer *&pContainer) {
    if (m_mapContainers.find(name) == m_mapContainers.end())
        return false;
    pContainer = m_mapContainers[name];
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::find_service(const std::string &name, ServiceLXD *&pService) {
    if (m_mapServices.find(name) == m_mapServices.end())
        return false;
    pService = m_mapServices[name];
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::get_all_profiles(std::vector<std::string> &vecProfiles, std::string &sError) {
    const std::string sUrl = "/1.0/profiles";
    nlohmann::json jsonReponse;

    if (!send_get_request(sUrl, jsonReponse, sError))
        return false;

    if (!jsonReponse.is_array()) {
        return false;
    }

    vecProfiles = jsonReponse.get<std::vector<std::string>>();
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::find_profile(const std::string &sName, std::string &sError) {
    std::vector<std::string> vecProfiles;

    if (!get_all_profiles(vecProfiles, sError)) {
        return false;
    }

    return !(std::find(vecProfiles.begin(), vecProfiles.end(), sName) == vecProfiles.end());
}

// ---------------------------------------------------------------------

static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
    ((std::string *) stream)->append((char *) ptr, 0, size * count);
    return size * count;
}

// ---------------------------------------------------------------------

static char errorBuffer[CURL_ERROR_SIZE];

bool EmployOrchestra::send_post_request_file(const std::string &sUrl, const std::string &sFile,
                                             std::string &sResponse,
                                             std::string &sError) {
    CURLcode ret;
    CURL *hnd;

    struct curl_slist *slist1;
    slist1 = nullptr;
    slist1 = curl_slist_append(slist1, "Content-Type: application/octet-stream");
    std::string hostname = m_sLxdAddress + sUrl;

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, sFile.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t) sFile.size());
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    std::string ssl_crt = m_sPathDirLxcSSL + "/client.crt";
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, ssl_crt.c_str());
    std::string ssl_key = m_sPathDirLxcSSL + "/client.key";
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, ssl_key.c_str());
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &sResponse);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);
    ret = curl_easy_perform(hnd);

    if (ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        WsjcppLog::err(TAG, " Failed send POST request with error " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    curl_easy_cleanup(hnd);
    return true;
}

bool EmployOrchestra::send_post_request(const std::string &sUrl, const nlohmann::json &jsonData,
                                        nlohmann::json &jsonResponse,
                                        std::string &sError) {
    CURLcode ret;
    CURL *hnd;

    std::string sData = jsonData.dump();
    hnd = curl_easy_init();
    std::string hostname = m_sLxdAddress + sUrl;
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, sData.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t) sData.size());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    std::string ssl_crt = m_sPathDirLxcSSL + "/client.crt";
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, ssl_crt.c_str());
    std::string ssl_key = m_sPathDirLxcSSL + "/client.key";
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, ssl_key.c_str());
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    std::string sResponse;
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &sResponse);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);
    ret = curl_easy_perform(hnd);

    if (ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        WsjcppLog::err(TAG, " Failed send POST request with error " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    jsonResponse = nlohmann::json::parse(sResponse);
    curl_easy_cleanup(hnd);

    if (!check_response(jsonResponse, sError)) {
        m_sLastError = sError;
        return false;
    }
    return true;
}

bool EmployOrchestra::send_patch_request(const std::string &sUrl, const nlohmann::json &jsonData,
                                         nlohmann::json &jsonResponse,
                                         std::string &sError) {
    CURLcode ret;
    CURL *hnd;

    std::string sData = jsonData.dump();
    hnd = curl_easy_init();
    std::string hostname = m_sLxdAddress + sUrl;
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, sData.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t) sData.size());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    std::string ssl_crt = m_sPathDirLxcSSL + "/client.crt";
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, ssl_crt.c_str());
    std::string ssl_key = m_sPathDirLxcSSL + "/client.key";
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, ssl_key.c_str());
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    std::string sResponse;
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &sResponse);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);
    ret = curl_easy_perform(hnd);

    if (ret != CURLE_OK) {
        m_sLastError = " Failed send PATCH request with error " + std::string(errorBuffer);
        WsjcppLog::err(TAG, m_sLastError);
        sError = std::string(errorBuffer);
        return false;
    }

    jsonResponse = nlohmann::json::parse(sResponse);
    curl_easy_cleanup(hnd);

    if (!check_response(jsonResponse, sError)) {
        m_sLastError = sError;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::send_put_request(const std::string &sUrl, const nlohmann::json &jsonData,
                                       nlohmann::json &jsonResponse,
                                       std::string &sError) {
    CURLcode ret;
    CURL *hnd;

    std::string sData = jsonData.dump();
    hnd = curl_easy_init();
    std::string hostname = m_sLxdAddress + sUrl;
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, sData.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t) sData.size());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    std::string ssl_crt = m_sPathDirLxcSSL + "/client.crt";
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, ssl_crt.c_str());
    std::string ssl_key = m_sPathDirLxcSSL + "/client.key";
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, ssl_key.c_str());
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    std::string response;
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);
    ret = curl_easy_perform(hnd);

    if (ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        WsjcppLog::err(TAG, "Failed send PUT request " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    jsonResponse = nlohmann::json::parse(response);
    curl_easy_cleanup(hnd);

    if (!check_response(jsonResponse, sError)) {
        m_sLastError = sError;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::send_get_request_raw(const std::string &sUrl, std::string &sResponse, std::string &sError) {
    // Without additional checks of response
    CURLcode ret;
    CURL *hnd;

    hnd = curl_easy_init();
    std::string hostname = m_sLxdAddress + sUrl;
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    std::string ssl_crt = m_sPathDirLxcSSL + "/client.crt";
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, ssl_crt.c_str());
    std::string ssl_key = m_sPathDirLxcSSL + "/client.key";
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, ssl_key.c_str());
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &sResponse);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);
    ret = curl_easy_perform(hnd);

    if (ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        WsjcppLog::err(TAG, "Failed send GET request " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    curl_easy_cleanup(hnd);
    return true;
}

bool EmployOrchestra::send_get_request(const std::string &sUrl, nlohmann::json &jsonResponse, std::string &sError) {

    std::string sResponse;
    if (!send_get_request_raw(sUrl, sResponse, sError)) {
        m_sLastError = sError;
        return false;
    }

    jsonResponse = nlohmann::json::parse(sResponse);

    if (!check_response(jsonResponse, sError)) {
        m_sLastError = sError;
        return false;
    }
    return true;
}

bool EmployOrchestra::send_get_request(const std::string &sUrl, std::string &sResponse, std::string &sError) {
    return send_get_request_raw(sUrl, sResponse, sError);
}

// ---------------------------------------------------------------------

bool EmployOrchestra::send_delete_request(const std::string &sUrl, nlohmann::json &jsonResponse, std::string &sError) {

    CURLcode ret;
    CURL *hnd;

    hnd = curl_easy_init();
    std::string hostname = m_sLxdAddress + sUrl;
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    std::string ssl_crt = m_sPathDirLxcSSL + "/client.crt";
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, ssl_crt.c_str());
    std::string ssl_key = m_sPathDirLxcSSL + "/client.key";
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, ssl_key.c_str());
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    std::string response;
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);
    ret = curl_easy_perform(hnd);

    if (ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        WsjcppLog::err(TAG, "Failed send DELETE request " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    jsonResponse = nlohmann::json::parse(response);
    curl_easy_cleanup(hnd);

    if (!check_response(jsonResponse, sError)) {
        m_sLastError = sError;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::pull_container_names() {
    std::string sUrl = "/1.0/containers";
    std::string sError;
    nlohmann::json jsonResponse;

    if (!send_get_request(sUrl, jsonResponse, sError)) {
        WsjcppLog::err(TAG, "Can't pull container names " + sError);
        return false;
    }

    std::vector<std::string> container_names = jsonResponse.at("metadata").get<std::vector<std::string>>();
    std::list<std::string> listNames;
    std::list<std::string> listRegistry = registry_names();

    for (const auto &it: container_names) {
        std::string str = std::string(it);
        std::string name = str.substr(15);

        if (name.length() <= 4)
            continue;

        if (name.substr(1, 4) == "fhq-") {
            auto exists = std::find(listRegistry.begin(), listRegistry.end(), name.substr(5));
            if (exists == listRegistry.end())
                listNames.push_back(name.substr(5));
        }
    }

    for (const auto& name : listNames) {
        auto *pContainer = new LXDContainer(name);
        m_mapContainers.insert(std::pair<std::string, LXDContainer *>(name, pContainer));

    }
    // Print container map
    //for (std::map<std::string, LXDContainer *>::iterator it=containers_map.begin(); it!=containers_map.end(); ++it)
    //    std::cout << it->first << " => " << it->second << '\n';
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::check_response(const nlohmann::json &jsonResponse, std::string &sError) {
    std::string error;

    if (!jsonResponse.at("error").get<std::string>().empty()) {
        error = jsonResponse.at("error").get<std::string>();
        sError = "request failed: " + error;
        WsjcppLog::err(TAG, sError);
        return false;
    }

    std::string metadata_error;
    if (jsonResponse.find("metadata") != jsonResponse.end())
        if (jsonResponse.at("metadata").find("err") != jsonResponse.at("metadata").end())
            metadata_error = jsonResponse.at("metadata").at("err").get<std::string>();

    if (!metadata_error.empty()) {
        error = metadata_error;
        sError = "request failed: " + error;
        WsjcppLog::err(TAG, sError);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::remove_container(const std::string &name, std::string &sError) {
    LXDContainer *pContainer;

    if (!find_container(name, pContainer)) {
        sError = "Not found container " + name;
        WsjcppLog::err(TAG, sError);
        return false;
    }

    if (pContainer->remove()) {
        m_mapContainers.erase(name);
        WsjcppLog::info(TAG, "Deleted container " + pContainer->full_name());
        delete pContainer;
        return true;
    }

    WsjcppLog::err(TAG, "Don't delete container " + pContainer->full_name());
    m_sLastError = "Don't delete container " + pContainer->full_name();
    sError = pContainer->get_error();
    return false;
}

// ---------------------------------------------------------------------

std::list<std::string> EmployOrchestra::registry_names() {
    std::list<std::string> container_names;
    for (const auto &container : m_mapContainers)
        container_names.push_back(container.first);
    return container_names;
}


// ---------------------------------------------------------------------
