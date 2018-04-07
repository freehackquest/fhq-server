#include <model_lxd_container.h>
#include <employ_orchestra.h>

LXDContainer::LXDContainer(QJsonObject jsonData){
    if(jsonData.contains("name")){
        name = jsonData.value("name").toString().toStdString();
    } else {
        error = "Container name is missing.";
    }

    EmploySettings *pSettings = findEmploy<EmploySettings>();
    path_dir_lxc_ssl = pSettings->getSettString("path_dir_lxc_ssl").toStdString();
    std::string lxd_server_ip = "https://" +  pSettings->getSettString("lxd_server_ip").toStdString();
    std::string lxd_server_port = pSettings->getSettString("lxd_server_port").toStdString();
    lxd_address = "https://" + lxd_server_ip + ":" + lxd_server_port;
    prefix = "fhq_";
}


static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
  ((std::string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}

static char errorBuffer[CURL_ERROR_SIZE];


std::string LXDContainer::get_name(){
    return name;
}

std::string LXDContainer::get_status(){
    return status;
}

std::string LXDContainer::get_IPv4(){
    return IPv4;
}

std::string LXDContainer::get_error(){
    return error;
}


nlohmann::json LXDContainer::state(){
    //TO DO return value

    CURLcode ret;
    CURL *hnd;
    std::string hostname = lxd_address + "/1.0/containers/xenial/state";

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, path_dir_lxc_ssl + "/client.crt");
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, path_dir_lxc_ssl + "/client.key");
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);
    ret = curl_easy_perform(hnd);

    if(ret != CURLE_OK) {
        fprintf(stderr, "Failed to create container [%s]\n", errorBuffer);
        error = errorBuffer;
        //return false;
    }

    curl_easy_cleanup(hnd);
    hnd = NULL;

    return nlohmann::json::object_t();

}


bool LXDContainer::create(std::string name){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers";
    std::string setings = "{\"name\": \"" + prefix + name + "\", \"source\": {\"type\": \"image\", \"protocol\": \"simplestreams\", \"server\": \"https://cloud-images.ubuntu.com/daily\", \"alias\": \"16.04\"}}";
    nlohmann::json res;
    std::string err;

    if (pOrchestra->send_post_request(address, setings, res, err)){
        this->error = err;
        return false;
    }


    //TO DO
    //Check response
    return true;

}

bool LXDContainer::start(){
    CURLcode ret;
    CURL *hnd;
    std::string readBuffer;

    std::string hostname = lxd_address + "/1.0/containers/" + name + "/state";

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "{\"action\": \"start\"}");
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE, (curl_off_t)19);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, path_dir_lxc_ssl + "/client.crt");
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, path_dir_lxc_ssl + "/client.key");
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);

    ret = curl_easy_perform(hnd);

    if(ret != CURLE_OK) {
      fprintf(stderr, "Failed to start container [%s]\n", errorBuffer);
      error = errorBuffer;
      return false;
    }

    curl_easy_cleanup(hnd);
    hnd = NULL;

    return true;

}

bool LXDContainer::stop(){
    CURLcode ret;
    CURL *hnd;
    std::string readBuffer;
    std::string hostname = "https://127.0.0.1:8443/1.0/containers/" + name + "/state";

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, hostname.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "{\"action\": \"stop\"}");
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE, (curl_off_t)18);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, path_dir_lxc_ssl + "/client.crt");
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, path_dir_lxc_ssl + "/client.key");
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    //Saving response
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);

    ret = curl_easy_perform(hnd);

    if(ret != CURLE_OK) {
        fprintf(stderr, "Failed to stop container [%s]\n", errorBuffer);
        error = errorBuffer;
        return false;
    }

    curl_easy_cleanup(hnd);
    hnd = NULL;

    return true;

}

bool LXDContainer::remove(){

}

std::string LXDContainer::exec(std::string){

}


bool LXDContainer::read_file(QFile & file)
{
    CURLcode ret;
    CURL *hnd;
    std::string readBuffer;

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, "https://127.0.0.1:8443/1.0/containers/ub-xen/files?path=/etc/hosts");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_SSLCERT, path_dir_lxc_ssl + "/client.crt");
    curl_easy_setopt(hnd, CURLOPT_SSLKEY, path_dir_lxc_ssl + "/client.key");
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    //Saving response
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errorBuffer);

    ret = curl_easy_perform(hnd);

    if(ret != CURLE_OK) {
        fprintf(stderr, "Failed to listing container [%s]\n", errorBuffer);
        error = errorBuffer;
        return false;
    }

    curl_easy_cleanup(hnd);
    hnd = NULL;

    return true;
}
