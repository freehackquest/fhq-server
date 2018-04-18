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


nlohmann::json LXDContainer::state(){
    //TO DO return value

    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + name + "/state";
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
    std::string metadata_error;
    if (res_json.at("metadata").find("err") != res_json.at("metadata").end())
        metadata_error = res_json.at("metadata").at("err").get<std::string>();
    if ( (res_json.at("error").get<std::string>() != "") || (metadata_error != "") ){
        error = metadata_error;
        Log::err(TAG, "Failed : " + error);
        return false;
    }

    return true;
}


bool LXDContainer::check_async_response(nlohmann::json operation_json){
    //Check async operation
    std::string metadata_error;
    if (operation_json.at("metadata").find("err") != operation_json.at("metadata").end())
        metadata_error = operation_json.at("metadata").at("err").get<std::string>();
    if ( (operation_json.at("error").get<std::string>() != "") || (metadata_error != "")){
        error = metadata_error;
        Log::err(TAG, "Operation is failed " + error);
        return false;
    }
}


bool LXDContainer::create(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers";
    std::string settings = "{\"name\": \"" + prefix + name + "\", \"source\": {\"type\": \"image\", \"protocol\": \"simplestreams\", \"server\": \"https://cloud-images.ubuntu.com/daily\", \"alias\": \"16.04\"}}";

    if (!pOrchestra->send_post_request(address, settings, response, error))
        return false;

    //Check response
    auto res_json = nlohmann::json::parse(response);
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

    return true;

}

bool LXDContainer::start(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + name + "/state";
    std::string settings = "{\"action\": \"start\"}";

    Log::info(TAG, "Send put request name:" + name + " address:" + address );
    if (!pOrchestra->send_put_request(address, settings, response, error))
        return false;

    Log::info(TAG, "Recive put request");

    auto res_json = nlohmann::json::parse(response);

    Log::info(TAG, "string to json");
    if (!check_response(res_json))
        return false;

    Log::info(TAG, "Checked response");

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

    Log::info(TAG, "Check async response");
    return true;

}

bool LXDContainer::stop(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + name + "/state";
    std::string settings = "{\"action\": \"stop\"}";

    if (!pOrchestra->send_put_request(address, settings, response, error)){
        return false;
    }

    auto res_json = nlohmann::json::parse(response);
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
    return true;
}

bool LXDContainer::remove(){
    return true;
}

std::string LXDContainer::exec(std::string command){
    return command;
}


bool LXDContainer::read_file(std::string path, QFile & file)
{
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + name + "/files?path=" + path;
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
