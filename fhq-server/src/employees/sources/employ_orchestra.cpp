#include <employ_orchestra.h>
#include <employ_settings.h>
#include <utils_lxd.h>

REGISTRY_EMPLOY(EmployOrchestra)

// ---------------------------------------------------------------------

#include <log.h>
#include <algorithm>

#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <QStringList>

// ---------------------------------------------------------------------

EmployOrchestra::EmployOrchestra()
    : EmployBase(EmployOrchestra::name(), {EmploySettings::name()}) {
    TAG = "EmployOrchestra";
}

// ---------------------------------------------------------------------

bool EmployOrchestra::init(){
    Log::info(TAG, "Start init settings");

    EmploySettings *pSettings = findEmploy<EmploySettings>();
    std::string lxd_mode = pSettings->getSettString("lxd_mode").toStdString();
    if (lxd_mode != "enabled")
        return true;

    m_sPathDirLxcSSL = pSettings->getSettString("path_dir_lxc_ssl").toStdString();
    std::string lxd_server_ip = pSettings->getSettString("lxd_server_ip").toStdString();
    std::string lxd_server_port = pSettings->getSettString("lxd_server_port").toStdString();
    m_sLxdAddress = "https://" + lxd_server_ip + ":" + lxd_server_port;
    std::string sError;
    m_bTrusted = UtilsLXDAuth::check_trust_certs(sError);
    if (!m_bTrusted){
        Log::err(TAG, "SSL certificates are not trusted. Please configure the connection with the LXD. Type ./fhq-server -mclxd");
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::initConnection(){

    Log::info(TAG, "Start init connection");

    //Pull existing containers
    if (m_bTrusted && !pull_container_names())
        return false;
    return true;
}

// ---------------------------------------------------------------------

std::string EmployOrchestra::lastError(){
    return m_sLastError;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::create_container(std::string sName, std::string &sError){
    Log::info(TAG, "Starting creation container " + sName);
    LXDContainer *pContainer = new LXDContainer(sName);

    if(pContainer->create()){
        Log::info(TAG, "Created container " + sName);
    }

    if(pContainer->get_error() != ""){
        sError = pContainer->get_error();
        return false;
    }

    m_mapContainers.insert( std::pair<std::string, LXDContainer *>(sName, pContainer) );
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::find_container(std::string name, LXDContainer *&pContainer){
    if (m_mapContainers.find(name) == m_mapContainers.end())
        return false;
    pContainer = m_mapContainers[name];
    return true;
}

// ---------------------------------------------------------------------

static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
  ((std::string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}

// ---------------------------------------------------------------------

static char errorBuffer[CURL_ERROR_SIZE];

bool EmployOrchestra::send_post_request(std::string sUrl, nlohmann::json jsonData, nlohmann::json & jsonResponse, std::string & sError){
    CURLcode ret;
    CURL *hnd;

    std::string sData = jsonData.dump();
    hnd = curl_easy_init();
    std::string hostname = m_sLxdAddress + sUrl;
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, sData.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)sData.size());
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

    if(ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        Log::err(TAG, " Failed send POST request with error " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    jsonResponse = nlohmann::json::parse(sResponse);

    curl_easy_cleanup(hnd);
    hnd = NULL;

    if (!check_response(jsonResponse, sError)){
        m_sLastError = sError;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::send_put_request(std::string sUrl, nlohmann::json jsonData, nlohmann::json & jsonResponse, std::string &sError){
    CURLcode ret;
    CURL *hnd;

    std::string sData = jsonData.dump();
    hnd = curl_easy_init();
    std::string hostname = m_sLxdAddress + sUrl;
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, sData.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)sData.size());
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

    if(ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        Log::err(TAG, "Failed send PUT request " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    jsonResponse = nlohmann::json::parse(response);
    curl_easy_cleanup(hnd);
    hnd = NULL;

    if (!check_response(jsonResponse, sError)){
        m_sLastError = sError;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::send_get_request(std::string sUrl, nlohmann::json &jsonResponse, std::string & sError){

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
    std::string response;
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);
    ret = curl_easy_perform(hnd);

    if(ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        Log::err(TAG, "Failed send GET request " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    jsonResponse = nlohmann::json::parse(response);
    curl_easy_cleanup(hnd);
    hnd = NULL;

    if (!check_response(jsonResponse, sError)){
        m_sLastError = sError;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::send_delete_request(std::string sUrl, nlohmann::json & jsonResponse, std::string & sError){

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

    if(ret != CURLE_OK) {
        m_sLastError = " Failed send POST request with error " + std::string(errorBuffer);
        Log::err(TAG, "Failed send DELETE request " + std::string(errorBuffer));
        sError = std::string(errorBuffer);
        return false;
    }

    jsonResponse = nlohmann::json::parse(response);

    curl_easy_cleanup(hnd);
    hnd = NULL;

    if (!check_response(jsonResponse, sError)){
        m_sLastError = sError;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::pull_container_names(){
    std::string sUrl = "/1.0/containers";
    std::string sError;
    nlohmann::json jsonResponse;

    if (!send_get_request(sUrl, jsonResponse, sError)){
        Log::err(TAG, "Can't pull container names " + sError);
        return false;
    }

    auto container_names = jsonResponse.at("metadata").get<std::vector<std::string>>();
    std::list<std::string> listNames;
    std::list<std::string> listRegistry = registry_names();

    for (auto it: container_names){
        std::string str = std::string(it);
        std::string name = str.substr(15);

        if (name.length() <= 4)
            continue;

        if (name.substr(1, 4) == "fhq-"){
            auto exists = std::find(listRegistry.begin(), listRegistry.end(), name.substr(5));
            if (exists == listRegistry.end())
                listNames.push_back(name.substr(5));
        }
    }

    for (auto name : listNames){
        LXDContainer *pContainer = new LXDContainer(name);
        m_mapContainers.insert( std::pair<std::string, LXDContainer *>(name, pContainer) );

    }
    // Print container map
    //for (std::map<std::string, LXDContainer *>::iterator it=containers_map.begin(); it!=containers_map.end(); ++it)
    //    std::cout << it->first << " => " << it->second << '\n';
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::check_response(nlohmann::json jsonResponse, std::string &sError){
    std::string error;

    if (jsonResponse.at("error").get<std::string>() != ""){
        error = jsonResponse.at("error").get<std::string>();
        sError = "request failed: " + error;
        Log::err(TAG, sError);
        return false;
    }

    std::string metadata_error;
    if (jsonResponse.find("metadata") != jsonResponse.end())
        if (jsonResponse.at("metadata").find("err") != jsonResponse.at("metadata").end())
            metadata_error = jsonResponse.at("metadata").at("err").get<std::string>();

    if (metadata_error != ""){
        error = metadata_error.c_str();
        sError = "request failed: " + error;
        Log::err(TAG, sError);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::remove_container(std::string name, std::string & sError){
    LXDContainer * pContainer;

    if (!find_container(name, pContainer)){
        sError = "Not found container " + name;
        Log::err(TAG, sError);
       return false;
    }

    if ( pContainer->remove() ){
        m_mapContainers.erase(name);
        // bad alloc for
        //delete container;
        Log::info(TAG, "Deleted container " + pContainer->full_name());
        return true;
    }

    Log::err(TAG, "Don't delete container " + pContainer->full_name());
    m_sLastError = "Don't delete container " + pContainer->full_name();
    sError = pContainer->get_error();
    return false;
}

// ---------------------------------------------------------------------

std::list<std::string> EmployOrchestra::registry_names(){
    std::list<std::string> container_names;
    for (std::map<std::string, LXDContainer *>::iterator it=m_mapContainers.begin(); it!=m_mapContainers.end(); ++it)
        container_names.push_back(it->first);
    return container_names;
}

// ---------------------------------------------------------------------
