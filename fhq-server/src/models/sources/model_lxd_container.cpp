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

nlohmann::json LXDContainer::state(){
    //TO DO return value

    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_get_request(sUrl, jsonResponse, m_sError)){
        return false;
    }


    //TO DO
    //Check response

    return jsonResponse;

}


bool LXDContainer::check_response(nlohmann::json res_json){
    if (res_json.at("error").get<std::string>() != ""){
        m_sError = res_json.at("error").get<std::string>();
        Log::err(TAG, "Failed check response: " + m_sError);
        return false;
    }

    std::string metadata_error;
    if (res_json.find("metadata") != res_json.end())
        if (res_json.at("metadata").find("err") != res_json.at("metadata").end())
            metadata_error = res_json.at("metadata").at("err").get<std::string>();

    if (metadata_error != ""){
        m_sError = metadata_error.c_str();
        Log::err(TAG, "Failed check response: " + m_sError);
        return false;
    }
    return true;
}


bool LXDContainer::check_async_response(nlohmann::json operation_json){
    //Check async operation
    if (operation_json.at("error").get<std::string>() != ""){
        m_sError = operation_json.at("error").get<std::string>();
        Log::err(TAG, "Operation is failed " + m_sError);
        return false;
    }

    std::string metadata_error;
    if (operation_json.find("metadata") != operation_json.end())
        if (operation_json.at("metadata").find("err") != operation_json.at("metadata").end())
            metadata_error = operation_json.at("metadata").at("err").get<std::string>();

    if (metadata_error != ""){
        m_sError = metadata_error.c_str();
        Log::err(TAG, "Operation is failed " + m_sError);
        return false;
    }

    return true;
}


bool LXDContainer::create(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers";
    std::string sData = "{\"name\": \"" + full_name() + "\", \"source\": {\"type\": \"image\", \"protocol\": \"simplestreams\", \"server\": \"https://cloud-images.ubuntu.com/daily\", \"alias\": \"16.04\"}}";
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_post_request(sUrl, sData, jsonResponse, m_sError))
        return false;

    //Check response
    if (!check_response(jsonResponse))
        return false;

    if (jsonResponse.at("type").get<std::string>() == "async"){
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))){
            Log::err(TAG, "Can\'t get operation " + m_sError);
            return false;
        }

        if (!check_async_response(jsonAsyncResponse))
            return false;
    }
    Log::info(TAG, "Created container " + full_name());
    return true;

}

bool LXDContainer::start(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";
    std::string sData = "{\"action\": \"start\"}";
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_put_request(sUrl, sData, jsonResponse, m_sError))
        return false;

    if (!check_response(jsonResponse))
        return false;
    //Log::info(TAG, "Async response " + res_json.dump());
    if (jsonResponse.at("type").get<std::string>() == "async"){
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        nlohmann::json jsonAsyncResponse;
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))){
            Log::err(TAG, "Can\'t get operation " + m_sError);
            return false;
        }

        if (check_async_response(jsonAsyncResponse)){
            Log::info(TAG, "Good check response");
        } else {
            Log::info(TAG, "Bad check response " + jsonAsyncResponse.dump());
        }
        if (!check_async_response(jsonAsyncResponse))
            return false;
    }

    Log::info(TAG, "Started container " + full_name());
    return true;

}

bool LXDContainer::stop(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";
    std::string sData = "{\"action\": \"stop\"}";
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_put_request(sUrl, sData, jsonResponse, m_sError)){
        return false;
    }

    if (!check_response(jsonResponse))
        return false;

    if (jsonResponse.at("type").get<std::string>() == "async"){
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))){
            Log::err(TAG, "Can\'t get operation " + m_sError);
            return false;
        }

        if (!check_async_response(jsonAsyncResponse))
            return false;
    }
    Log::info(TAG, "Stoped container " + full_name());
    return true;
}

bool LXDContainer::remove(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + full_name();
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_delete_request(address, jsonResponse, m_sError)){
        return false;
    }

    if (!check_response(jsonResponse))
        return false;

    if (jsonResponse.at("type").get<std::string>() == "async"){
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))){
            Log::err(TAG, "Can\'t get operation " + m_sError);
            return false;
        }

        if (!check_async_response(jsonAsyncResponse))
            return false;
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

    //TO DO
    //Check response

    return true;
}
