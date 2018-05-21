#include <model_lxd_container.h>
#include <employ_orchestra.h>

LXDContainer::LXDContainer(std::string name_of_container){
    name = name_of_container;
    prefix = "fhq-";
}

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
    return m_sError;
}

std::string LXDContainer::full_name(){
    return prefix + name;
}

bool LXDContainer::state(nlohmann::json &jsonState){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";

    if (!pOrchestra->send_get_request(sUrl, jsonState, m_sError)){
        return false;
    }
    return true;
}

bool LXDContainer::create(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers";
    auto jsonData = R"(
    {
        "name": "full_name",
        "source": {
            "type": "image",
            "protocol": "simplestreams",
            "server": "https://cloud-images.ubuntu.com/daily",
            "alias": "16.04"
        }
    })"_json;
    jsonData["name"] = full_name();
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_post_request(sUrl, jsonData, jsonResponse, m_sError))
        return false;

    if (jsonResponse.at("type").get<std::string>() == "async"){
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))){
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
    }
    Log::info(TAG, "Created container " + full_name());
    return true;

}

bool LXDContainer::start(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";
    auto jsonData = R"(
        {
            "action": "start"
        }
    )"_json;
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_put_request(sUrl, jsonData, jsonResponse, m_sError))
        return false;

    if (jsonResponse.at("type").get<std::string>() == "async"){
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        nlohmann::json jsonAsyncResponse;
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))){
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
    }

    Log::info(TAG, "Started container " + full_name());
    return true;

}

bool LXDContainer::stop(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";
    auto jsonData = R"(
        {
            "action": "stop"
        }
    )"_json;
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_put_request(sUrl, jsonData, jsonResponse, m_sError)){
        return false;
    }

    if (jsonResponse.at("type").get<std::string>() == "async"){
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))){
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
    }
    Log::info(TAG, "Stoped container " + full_name());
    return true;
}

bool LXDContainer::remove(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name();
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_delete_request(sUrl, jsonResponse, m_sError)){
        return false;
    }

    if (jsonResponse.at("type").get<std::string>() == "async"){
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))){
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
    }
    Log::info(TAG, "Deleted container " + full_name());
    return true;
}

std::string LXDContainer::exec(std::string command){
    return command;
}


bool LXDContainer::read_file(std::string path, QFile & file)
{
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/files?path=" + path;
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_get_request(sUrl, jsonResponse, m_sError))
        return false;

    return true;
}
