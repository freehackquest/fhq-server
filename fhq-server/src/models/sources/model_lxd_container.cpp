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


bool LXDContainer::create(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers";
    std::string settings = "{\"name\": \"" + prefix + name + "\", \"source\": {\"type\": \"image\", \"protocol\": \"simplestreams\", \"server\": \"https://cloud-images.ubuntu.com/daily\", \"alias\": \"16.04\"}}";
    if (!pOrchestra->send_post_request(address, settings, response, error))
        return false;

    //Check response
    auto res_json = nlohmann::json::parse(response);
    std::string metadata_error = res_json.at("metadata").at("err").get<std::string>();
    if ( (res_json.at("error").get<std::string>() != "") || (metadata_error != "") ){
        error = metadata_error;
        Log::err(TAG, "Failed create : " + error);
        return false;
    }

    response = "";
    std::string operation_id = res_json.at("operation").get<std::string>();
    if ( (operation_id != "") && (!pOrchestra->send_get_request(operation_id + "/wait", response, error))){
        Log::err(TAG, "Can\'t get operation " + error);
        return false;
    }

    //Check async operation
    auto operation_json = nlohmann::json::parse(response);
    metadata_error = operation_json.at("metadata").at("err").get<std::string>();
    if ( (operation_json.at("error").get<std::string>() != "") || (metadata_error != "")){
        error = metadata_error;
        Log::err(TAG, "Operation is failed " + error);
        return false;
    }

    return true;

}

bool LXDContainer::start(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + name + "/state";
    std::string settings = "{\"action\": \"start\"}";
    std::string res;
    std::string err;

    if (!pOrchestra->send_put_request(address, settings, res, err)){
        error = err;
        return false;
    }


    //TO DO
    //Check response
    return true;

}

bool LXDContainer::stop(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + name + "/state";
    std::string settings = "{\"action\": \"stop\"}";
    std::string res;
    std::string err;

    if (!pOrchestra->send_put_request(address, settings, res, err)){
        error = err;
        return false;
    }


    //TO DO
    //Check response
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
