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
    return error;
}

std::string LXDContainer::full_name(){
    return prefix + name;
}

nlohmann::json LXDContainer::state(){
    //TO DO return value

    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + full_name() + "/state";
    std::string res;
    std::string err;

    if (!pOrchestra->send_get_request(address, res, err)){
        error = err;
        return false;
    }


    //TO DO
    //Check response

    return nlohmann::json::object_t();

}


bool LXDContainer::check_response(nlohmann::json res_json){
    if (res_json.at("error").get<std::string>() != ""){
        error = res_json.at("error").get<std::string>();
        Log::err(TAG, "Failed check response: " + error);
        return false;
    }

    std::string metadata_error;
    if (res_json.find("metadata") != res_json.end())
        if (res_json.at("metadata").find("err") != res_json.at("metadata").end())
            metadata_error = res_json.at("metadata").at("err").get<std::string>();

    if (metadata_error != ""){
        error = metadata_error.c_str();
        Log::err(TAG, "Failed check response: " + error);
        return false;
    }
    return true;
}


bool LXDContainer::check_async_response(nlohmann::json operation_json){
    //Check async operation
    if (operation_json.at("error").get<std::string>() != ""){
        error = operation_json.at("error").get<std::string>();
        Log::err(TAG, "Operation is failed " + error);
        return false;
    }

    std::string metadata_error;
    if (operation_json.find("metadata") != operation_json.end())
        if (operation_json.at("metadata").find("err") != operation_json.at("metadata").end())
            metadata_error = operation_json.at("metadata").at("err").get<std::string>();

    if (metadata_error != ""){
        error = metadata_error.c_str();
        Log::err(TAG, "Operation is failed " + error);
        return false;
    }

    return true;
}


bool LXDContainer::create(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers";
    std::string settings = "{\"name\": \"" + full_name() + "\", \"source\": {\"type\": \"image\", \"protocol\": \"simplestreams\", \"server\": \"https://cloud-images.ubuntu.com/daily\", \"alias\": \"16.04\"}}";
    std::string response;

    if (!pOrchestra->send_post_request(address, settings, response, error))
        return false;

    //Check response
    auto res_json = nlohmann::json::parse(response);
    response = "";
    if (!check_response(res_json))
        return false;

    if (res_json.at("type").get<std::string>() == "async"){
        response = "";
        std::string operation_id = res_json.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", response, error))){
            Log::err(TAG, "Can\'t get operation " + error);
            return false;
        }

        auto operation_json = nlohmann::json::parse(response);
        if (!check_async_response(operation_json))
            return false;
    }
    Log::info(TAG, "Created container " + full_name());
    return true;

}

bool LXDContainer::start(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + full_name() + "/state";
    std::string settings = "{\"action\": \"start\"}";
    std::string response;
    Log::info(TAG, "Before send put request");
    if (!pOrchestra->send_put_request(address, settings, response, error))
        return false;

    auto res_json = nlohmann::json::parse(response);
    response = "";
    if (!check_response(res_json))
        return false;
    //Log::info(TAG, "Async response " + res_json.dump());
    if (res_json.at("type").get<std::string>() == "async"){
        std::string operation_id = res_json.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", response, error))){
            Log::err(TAG, "Can\'t get operation " + error);
            return false;
        }

        auto operation_json = nlohmann::json::parse(response);
        if (check_async_response(operation_json)){
            Log::info(TAG, "Good check response");
        } else {
            Log::info(TAG, "Bad check response " + operation_json.dump());
        }
        if (!check_async_response(operation_json))
            return false;
    }

    Log::info(TAG, "Started container " + full_name());
    return true;

}

bool LXDContainer::stop(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + full_name() + "/state";
    std::string settings = "{\"action\": \"stop\"}";
    std::string response;

    if (!pOrchestra->send_put_request(address, settings, response, error)){
        return false;
    }

    auto res_json = nlohmann::json::parse(response);
    response = "";
    if (!check_response(res_json))
        return false;

    if (res_json.at("type").get<std::string>() == "async"){
        response = "";
        std::string operation_id = res_json.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", response, error))){
            Log::err(TAG, "Can\'t get operation " + error);
            return false;
        }

        auto operation_json = nlohmann::json::parse(response);
        if (!check_async_response(operation_json))
            return false;
    }
    Log::info(TAG, "Stoped container " + full_name());
    return true;
}

bool LXDContainer::remove(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + full_name();
    std::string response;

    if (!pOrchestra->send_delete_request(address, response, error)){
        return false;
    }

    auto res_json = nlohmann::json::parse(response);
    response = "";
    if (!check_response(res_json))
        return false;


    if (res_json.at("type").get<std::string>() == "async"){
        response = "";
        std::string operation_id = res_json.at("operation").get<std::string>();
        if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", response, error))){
            Log::err(TAG, "Can\'t get operation " + error);
            return false;
        }

        auto operation_json = nlohmann::json::parse(response);
        if (!check_async_response(operation_json))
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
    std::string address = "/1.0/containers/" + full_name() + "/files?path=" + path;
    std::string res;
    std::string err;

    if (!pOrchestra->send_get_request(address, res, err)){
        error = err;
        return false;
    }


    //TO DO
    //Check response

    return true;
}
