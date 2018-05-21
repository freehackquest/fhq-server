#ifndef EMPLOY_ORCHESTRA
#define EMPLOY_ORCHESTRA

#include <employees.h>
#include <model_lxd_container.h>

#include <list>
#include <string>
#include <map>
#include <curl/curl.h>


class EmployOrchestra : public EmployBase {
public:
    EmployOrchestra();
    static std::string name() { return "EmployOrchestra"; }
    virtual bool init();

    bool initConnection();
    bool create_container(std::string sName, std::string &sError);
    bool check_response(nlohmann::json jsonResponse, std::string &sError);
    bool find_container(std::string sName, LXDContainer *&pContainer);
    bool remove_container(std::string sName, std::string &sError);
    bool send_post_request(std::string sUrl, nlohmann::json jsonData, nlohmann::json &jsonResponse, std::string &sError);
    bool send_put_request(std::string sUrl, nlohmann::json jsonData, nlohmann::json &jsonResponse, std::string &sError);
    bool send_get_request(std::string sUrl, nlohmann::json &jsonResponse, std::string &sError);
    bool send_delete_request(std::string sUrl, nlohmann::json &jsonResponse, std::string &sError);

    std::list<std::string> registry_names();
    std::string lastError();

private:
    std::map<std::string, LXDContainer *> m_mapContainers;
    std::string m_sPathDirLxcSSL;
    std::string m_sLxdAddress;
    std::string m_sLastError;
    std::string TAG;

    bool m_bTrusted;

    bool pull_container_names();
};

#endif // EMPLOY_ORCHESTRA

