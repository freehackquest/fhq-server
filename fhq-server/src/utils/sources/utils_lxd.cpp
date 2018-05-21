#include <utils_lxd.h>
#include <json.hpp>
#include <employ_orchestra.h>
#include <employees.h>


bool UtilsLXDAuth::check_trust_certs(std::string &sError){
    std::string sUrl = "/1.0";
    nlohmann::json jsonResponse;
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()){
        sError = "Can\'t connect to LXD server";
        return false;
    }

    if (!pOrchestra->send_get_request(sUrl, jsonResponse, sError))
        return false;

    if ( (jsonResponse["metadata"]["auth"].is_string()) && (jsonResponse["metadata"]["auth"] == "trusted" ))
        return true;
    else
        return false;
}

bool UtilsLXDAuth::connect_with_lxd(std::string sPass,  std::string &sError){
    bool bTrusted = check_trust_certs(sError);

    if (sError != ""){
        bTrusted = "Can't get info about client cert";
        Log::err(std::string("UtilsLXDAuth"), "Can't get info about client cert");
        return false;
    }

    if (!bTrusted){
        if (!set_trusted(sPass, sError)){
            Log::err("UtilsLXDAuth", "Can't set trusted certs" + sError);
        }else{
            bTrusted = true;
        }
    }

    return bTrusted;
}

bool UtilsLXDAuth::set_trusted(std::string sPass, std::string &sError){
    // std::cout << "[set_trusted] response: " << "\n";
    std::string sUrl = "/1.0/certificates";
    auto jsonData = R"(
    {
        "type": "client",
        "password": "change it"
    })"_json;
    jsonData["password"] = sPass;
    nlohmann::json jsonResponse;
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()){
        sError = "Can\'t connect to LXD server";
        return false;
    }

    if (!pOrchestra->send_post_request(sUrl, jsonData, jsonResponse, sError))
        return false;

    return true;
}
