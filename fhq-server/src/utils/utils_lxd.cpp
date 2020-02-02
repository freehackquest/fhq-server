#include <utility>

#include <utility>

#include <utils_lxd.h>
#include <json.hpp>
#include <employ_orchestra.h>
#include <wsjcpp_employees.h>


bool UtilsLXDAuth::check_trust_certs(std::string &sError) {
    std::string sUrl = "/1.0";
    nlohmann::json jsonResponse;
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()) {
        sError = "Can\'t connect to LXD server";
        return false;
    }

    if (!pOrchestra->send_get_request(sUrl, jsonResponse, sError))
        return false;

    return (jsonResponse["metadata"]["auth"].is_string()) && (jsonResponse["metadata"]["auth"] == "trusted" );
}

bool UtilsLXDAuth::connect_with_lxd(const std::string &sPass, std::string &sError) {
    bool bTrusted = check_trust_certs(sError);

    if (!sError.empty()) {
        WSJCppLog::err(std::string("UtilsLXDAuth"), "Can't get info about client cert");
        return false;
    }

    if (!bTrusted) {
        if (!set_trusted(sPass, sError)) {
            WSJCppLog::err("UtilsLXDAuth", "Can't set trusted certs" + sError);
        } else {
            bTrusted = true;
        }
    }

    return bTrusted;
}

bool UtilsLXDAuth::set_trusted(const std::string &sPass, std::string &sError) {
    std::string sUrl = "/1.0/certificates";
    nlohmann::json jsonData = R"(
    {
        "type": "client",
        "password": "change it"
    })"_json;
    jsonData["password"] = sPass;
    nlohmann::json jsonResponse;
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()) {
        sError = "Can\'t connect to LXD server";
        return false;
    }

    return pOrchestra->send_post_request(sUrl, jsonData, jsonResponse, sError);

}
