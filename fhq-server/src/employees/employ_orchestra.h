#ifndef EMPLOY_ORCHESTRA
#define EMPLOY_ORCHESTRA

#include <wsjcpp_employees.h>
#include <model_lxd_orchestra.h>

#include <list>
#include <string>
#include <map>
#include <curl/curl.h>
#include <future>


class EmployOrchestra : public WSJCppEmployBase {
public:
    EmployOrchestra();

    static std::string name() { return "EmployOrchestra"; }

    bool init() override;

    bool initConnection();
    bool create_container(const std::string &sName, std::string &sError);
    bool create_service(const ServiceConfig &serviceReq, std::string &sError);
    bool create_service(const nlohmann::json &jsonConfig, std::string &sError);
    bool check_response(const nlohmann::json &jsonResponse, std::string &sError);
    bool find_container(const std::string &sName, LXDContainer *&pContainer);
    bool find_service(const std::string &sName, ServiceLXD *&pService);
    bool remove_container(const std::string &sName, std::string &sError);
    bool get_all_profiles(std::vector<std::string> &vecProfiles, std::string &sError);
    bool find_profile(const std::string &sName, std::string &sError);
    bool send_post_request_file(const std::string &sUrl, const std::string &sFile, std::string &sResponse,
                                std::string &sError);
    bool send_post_request(const std::string &sUrl, const nlohmann::json &jsonData, nlohmann::json &jsonResponse,
                           std::string &sError);
    bool send_patch_request(const std::string &sUrl, const nlohmann::json &jsonData, nlohmann::json &jsonResponse,
                            std::string &sError);
    bool send_put_request(const std::string &sUrl, const nlohmann::json &jsonData, nlohmann::json &jsonResponse,
                          std::string &sError);
    bool send_get_request(const std::string &sUrl, nlohmann::json &jsonResponse, std::string &sError);
    bool send_get_request(const std::string &sUrl, std::string &sResponse, std::string &sError);
    bool send_get_request_raw(const std::string &sUrl, std::string &sResponse, std::string &sError);
    bool send_delete_request(const std::string &sUrl, nlohmann::json &jsonResponse, std::string &sError);

    std::list<std::string> registry_names();
    std::string lastError();

private:
    std::map<std::string, LXDContainer *> m_mapContainers;
    std::map<std::string, ServiceLXD *> m_mapServices;
    std::string m_sPathDirLxcSSL;
    std::string m_sLxdAddress;
    std::string m_sLastError;
    std::string TAG;

    bool m_bTrusted;

    bool pull_container_names();
};

#endif // EMPLOY_ORCHESTRA

