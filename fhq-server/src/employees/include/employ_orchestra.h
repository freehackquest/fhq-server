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
    bool find_container(std::string sName);
    bool remove_container(std::string sName, std::string &sError);
    bool send_post_request(std::string sUrl, std::string sData, nlohmann::json &jsonResponse, std::string & sError);
    bool send_put_request(std::string sUrl, std::string sData, nlohmann::json & jsonResponse, std::string & sError);
    bool send_get_request(std::string sUrl, nlohmann::json &jsonResponse, std::string & sError);
    bool send_delete_request(std::string sUrl, nlohmann::json &jsonResponse, std::string & sError);

    std::list<std::string> registry_names();
    LXDContainer * get_container(std::string sName);

private:
    std::map<std::string, LXDContainer *> containers_map;
    std::string sPathDirLxcSSL;
    std::string sLxdAddress;
    std::string TAG;

    bool trusted;

    bool pull_container_names();
    bool check_response(nlohmann::json res_json, std::string error);
    bool check_async_response(nlohmann::json operation_json, std::string error);
    bool set_trusted(std::string password, std::string & error);
    bool check_trust_certs(std::string &error);
    bool connect_with_lxd(std::string lxd_passwd);
};

#endif // EMPLOY_ORCHESTRA

