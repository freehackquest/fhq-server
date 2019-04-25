#include <utility>

#include <model_lxd_container.h>
#include <employ_orchestra.h>
#include <include/model_lxd_container.h>
#include <iomanip>
#include <vector>
#include <iostream>
#include <iterator>
#include <sstream>


ServiceRequest::ServiceRequest(nlohmann::json jsonConfig) {
    // Smells awful
    name = jsonConfig.at("name").get<std::string>();
    if (jsonConfig.find("type") != jsonConfig.end()) {
        type = jsonConfig.at("type").get<std::string>();
    }
    if (jsonConfig.find("game") != jsonConfig.end()) {
        game = jsonConfig.at("game").get<std::string>();
    }
    if (jsonConfig.find("author") != jsonConfig.end()) {
        author = jsonConfig.at("author").get<std::string>();
    }
    if (jsonConfig.find("version") != jsonConfig.end()) {
        version = jsonConfig.at("version").get<std::string>();
    }
    if (jsonConfig.find("start") != jsonConfig.end()) {
        start = jsonConfig.at("start").get<std::string>();
    }
    if (jsonConfig.find("build") != jsonConfig.end()) {
        build = jsonConfig.at("build").get<std::string>();
    }
    if (jsonConfig.find("port_proto") != jsonConfig.end()) {
        port_proto = jsonConfig.at("port_proto").get<std::string>();
    }
    if (jsonConfig.find("port_number") != jsonConfig.end()) {
        port_number = jsonConfig.at("port_number").get<int>();
    }
}


LXDContainer::LXDContainer(const std::string &name_of_container) {
    name = name_of_container;
}

LXDContainer::LXDContainer(const ServiceRequest &containerReq) {
    name = containerReq.name;
    m_sType = containerReq.type;
    m_sVersion = containerReq.version;
    m_sAuthor = containerReq.author;
    m_sGame = containerReq.game;
}

std::string LXDContainer::get_name() const {
    return name;
}

std::string LXDContainer::get_status() const {
    return status;
}

std::string LXDContainer::get_IPv4() const {
    return IPv4;
}

std::string LXDContainer::get_error() const {
    return m_sError;
}

std::string LXDContainer::full_name() const {
    return prefix + name;
}

bool LXDContainer::state(nlohmann::json &jsonState) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";

    return pOrchestra->send_get_request(sUrl, jsonState, m_sError);
}

bool LXDContainer::create() {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
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

    if (jsonResponse.at("type").get<std::string>() == "async") {
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ((!operation_id.empty()) &&
            (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))) {
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
    }
    Log::info(TAG, "Created container " + full_name());
    return true;

}

bool LXDContainer::start() {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";
    auto jsonData = R"(
        {
            "action": "start"
        }
    )"_json;
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_put_request(sUrl, jsonData, jsonResponse, m_sError))
        return false;

    if (jsonResponse.at("type").get<std::string>() == "async") {
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        nlohmann::json jsonAsyncResponse;
        if ((!operation_id.empty()) &&
            (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))) {
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
    }

    Log::info(TAG, "Started container " + full_name());
    return true;

}

bool LXDContainer::stop() {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    std::string sUrl = "/1.0/containers/" + full_name() + "/state";
    auto jsonData = R"(
        {
            "action": "stop"
        }
    )"_json;
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_put_request(sUrl, jsonData, jsonResponse, m_sError)) {
        return false;
    }

    if (jsonResponse.at("type").get<std::string>() == "async") {
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ((!operation_id.empty()) &&
            (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))) {
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
    }
    Log::info(TAG, "Stopped container " + full_name());
    return true;
}

bool LXDContainer::remove() {
    auto pOrchestra = findEmploy<EmployOrchestra>();
    auto sUrl = "/1.0/containers/" + full_name();
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_delete_request(sUrl, jsonResponse, m_sError)) {
        return false;
    }

    if (jsonResponse.at("type").get<std::string>() == "async") {
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ((!operation_id.empty()) &&
            (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))) {
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
    }
    Log::info(TAG, "Deleted container " + full_name());
    return true;
}

std::vector<std::string> LXDContainer::split(const std::string &str) {
    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> tokens(beg, end);
    return tokens;
}

bool LXDContainer::exec(const std::string &sCommand) {
    // Allows to execute shell script. Does not return command stdout.
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    auto sUrl = "/1.0/containers/" + full_name() + "/exec";
    nlohmann::json jsonData = R"(
        {
            "command": ["/bin/uname", "-a"],
            "environment": {},
            "wait-for-websocket": false,
            "interactive": false
        }
    )"_json;
    jsonData["command"] = nlohmann::json(split(sCommand));
    nlohmann::json jsonResponse;

    if (!pOrchestra->send_post_request(sUrl, jsonData, jsonResponse, m_sError)) {
        return false;
    }

    if (jsonResponse.at("type").get<std::string>() == "async") {
        nlohmann::json jsonAsyncResponse;
        std::string operation_id = jsonResponse.at("operation").get<std::string>();
        if ((!operation_id.empty()) &&
            (!pOrchestra->send_get_request(operation_id + "/wait", jsonAsyncResponse, m_sError))) {
            Log::err(TAG, "The asynchronous " + m_sError);
            return false;
        }
        if (jsonAsyncResponse["metadata"]["metadata"]["return"] != 0) {
            m_sError = "The command '" + sCommand + "' could not be executed in " + full_name() + " container";
            Log::err(TAG, "Failed to execute " + sCommand + " in container " + full_name());
            return false;
        }
    }
    Log::info(TAG, "Success execution " + sCommand + " in container " + full_name());
    return true;
}


bool LXDContainer::read_file(const std::string &sPath, std::string &sRawData) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    auto sUrl = "/1.0/containers/" + full_name() + "/files?path=" + sPath;

    return pOrchestra->send_get_request(sUrl, sRawData, m_sError);
}

bool LXDContainer::push_file(const std::string &sPath, const std::string &sRawData) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    auto sUrl = "/1.0/containers/" + full_name() + "/files?path=" + sPath;
    std::string sResponse;

    if (!pOrchestra->send_post_request_file(sUrl, sRawData, sResponse, m_sError)) {
        return false;
    }

    if (nlohmann::json::accept(sResponse)) {
        m_sError = "Response is json";
        return false;
    }

    return true;
}

std::string LXDContainer::get_result() const {
    return m_sResult;
}

std::string LXDContainer::get_port() {
    return m_sPort;
}

bool LXDContainer::open_port(const std::string &sPort, const std::string &sProto) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    auto sUrl = "/1.0/containers/" + full_name();
    nlohmann::json jsonResponse;
    nlohmann::json jsonRequest;

    jsonRequest["devices"] = {};
    jsonRequest["devices"][sProto + sPort]["connect"] = sProto + ":localhost:" + sPort;
    jsonRequest["devices"][sProto + sPort]["listen"] = sProto + ":0.0.0.0:" + sPort;
    jsonRequest["devices"][sProto + sPort]["type"] = "proxy";

    if (!pOrchestra->send_patch_request(sUrl, jsonRequest, jsonResponse, m_sError)) {
        return false;
    }
    m_sPort = sPort;
    m_sProtoPort = sProto;
    return true;
}

bool LXDContainer::open_port(const int &nPort, const std::string &sProto) {
    auto sPort = std::to_string(nPort);
    return open_port(sPort, sProto);
}
