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
    nlohmann::json res;
    std::string err;

    if (pOrchestra->send_get_request(address, res, err)){
        error = err;
        return false;
    }


    //TO DO
    //Check response

    return nlohmann::json::object_t();

}


bool LXDContainer::create(std::string name){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers";
    std::string settings = "{\"name\": \"" + prefix + name + "\", \"source\": {\"type\": \"image\", \"protocol\": \"simplestreams\", \"server\": \"https://cloud-images.ubuntu.com/daily\", \"alias\": \"16.04\"}}";
    nlohmann::json res;
    std::string err;

    if (pOrchestra->send_post_request(address, settings, res, err)){
        error = err;
        return false;
    }


    //TO DO
    //Check response
    return true;

}

bool LXDContainer::start(){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + name + "/state";
    std::string settings = "{\"action\": \"start\"}";
    nlohmann::json res;
    std::string err;

    if (pOrchestra->send_put_request(address, settings, res, err)){
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
    nlohmann::json res;
    std::string err;

    if (pOrchestra->send_put_request(address, settings, res, err)){
        error = err;
        return false;
    }


    //TO DO
    //Check response
    return true;
}

bool LXDContainer::remove(){

}

std::string LXDContainer::exec(std::string){

}


bool LXDContainer::read_file(std::string path, QFile & file)
{
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    std::string address = "/1.0/containers/" + name + "/files?path=" + path;
    nlohmann::json res;
    std::string err;

    if (pOrchestra->send_get_request(address, res, err)){
        error = err;
        return false;
    }


    //TO DO
    //Check response

    return true;
}
