#include <employ_orchestra.h>
#include <employ_settings.h>

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
    Log::info(TAG, "Nothing");
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::initConnection(){

    Log::info(TAG, "Start init settings");

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    m_sPathDirLxcSSL = pSettings->getSettString("path_dir_lxc_ssl").toStdString();
    std::string lxd_server_ip = pSettings->getSettString("lxd_server_ip").toStdString();
    std::string lxd_server_port = pSettings->getSettString("lxd_server_port").toStdString();
    m_sLxdAddress = "https://" + lxd_server_ip + ":" + lxd_server_port;
    QString lxd_passwd = pSettings->getSettString("lxd_passwd");

    if (!connect_with_lxd(lxd_passwd.toStdString())){
        Log::err(TAG, "Can't set trusted connection");
        m_sLastError = "Can't set trusted connection";
        return false;
    }

    //TO DO
    //Connect to LXD in first time for add SSL cert
    //

    Log::info(TAG, "Pull containers");
    //Pull existing containers
    if (!pull_container_names()){

        return false;
    }
    Log::info(TAG, "Pulled containers");
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

LXDContainer * EmployOrchestra::get_container(std::string name){
    LXDContainer * container = m_mapContainers[name];
    return container;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::find_container(std::string name){
    if (m_mapContainers.find(name) == m_mapContainers.end())
        return false;

    return true;
}

// ---------------------------------------------------------------------

static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
  ((std::string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}

// ---------------------------------------------------------------------

static char errorBuffer[CURL_ERROR_SIZE];

bool EmployOrchestra::send_post_request(std::string sUrl, std::string sData, nlohmann::json & jsonResponse, std::string & sError){
    CURLcode ret;
    CURL *hnd;

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

    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::send_put_request(std::string sUrl, std::string sData, nlohmann::json & jsonResponse, std::string & sError){
    CURLcode ret;
    CURL *hnd;

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

    if (!check_response(jsonResponse, sError)){
        Log::err(TAG, "Can't pull container names " + sError);
        return false;
    }

    if (jsonResponse.find("metadata") == jsonResponse.end())
        return false;

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

bool EmployOrchestra::check_response(nlohmann::json res_json, std::string error){
    if (res_json.at("error").get<std::string>() != ""){
        error = res_json.at("error").get<std::string>();
        Log::err(TAG, "Failed check response: " + error);
        m_sLastError = "Failed check response: " + error;
        return false;
    }

    std::string metadata_error;
    if (res_json.find("metadata") != res_json.end())
        if (res_json.at("metadata").find("err") != res_json.at("metadata").end())
            metadata_error = res_json.at("metadata").at("err").get<std::string>();

    if (metadata_error != ""){
        error = metadata_error.c_str();
        Log::err(TAG, "Failed check response: " + error);
        m_sLastError = "Failed check response: " + error;
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::check_async_response(nlohmann::json operation_json, std::string error){
    //Check async operation
    if (operation_json.at("error").get<std::string>() != ""){
        error = operation_json.at("error").get<std::string>();
        Log::err(TAG, "Operation is failed " + error);
        m_sLastError = "Operation is failed " + error;
        return false;
    }

    std::string metadata_error;
    if (operation_json.find("metadata") != operation_json.end())
        if (operation_json.at("metadata").find("err") != operation_json.at("metadata").end())
            metadata_error = operation_json.at("metadata").at("err").get<std::string>();

    if (metadata_error != ""){
        error = metadata_error.c_str();
        Log::err(TAG, "Operation is failed " + error);
        m_sLastError = "Operation is failed " + error;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::remove_container(std::string name, std::string & sError){
    LXDContainer * container;
    container = get_container(name);
    if ( container->remove() ){
        m_mapContainers.erase(name);
        // bad alloc for
        //delete container;
        Log::info(TAG, "Deleted container " + container->full_name());
        return true;
    }

    Log::err(TAG, "Don't delete container " + container->full_name());
    m_sLastError = "Don't delete container " + container->full_name();
    sError = container->get_error();
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

bool EmployOrchestra::set_trusted(std::string password, std::string & error){
    // std::cout << "[set_trusted] response: " << "\n";
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/certificates";
    std::string sData = "{\"type\": \"client\", \"password\": \"" + password + "\"}";
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_post_request(sUrl, sData, jsonResponse, error))
        return false;

    if (!check_response(jsonResponse, error))
        return false;

    return true;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::connect_with_lxd(std::string lxd_passwd){
    std::string error;
    m_bTrusted = check_trust_certs(error);

    if (error != ""){
        m_sLastError = "Can't get info about client cert";
        Log::err(TAG, "Can't get info about client cert");
        return false;
    }

    if (!m_bTrusted){
        if (!set_trusted(lxd_passwd, error)){
            m_sLastError = "Can't set trusted certs" + error;
            Log::err(TAG, "Can't set trusted certs" + error);
        }else{
            m_bTrusted = true;
        }
    }

    return m_bTrusted;
}

// ---------------------------------------------------------------------

bool EmployOrchestra::check_trust_certs(std::string & error){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0";
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_get_request(sUrl, jsonResponse, error))
        return false;

    if (!check_response(jsonResponse, error))
        return false;

    if ( (jsonResponse["metadata"]["auth"].is_string()) && (jsonResponse["metadata"]["auth"] == "trusted" ))
        return true;
    else
        return false;
}
