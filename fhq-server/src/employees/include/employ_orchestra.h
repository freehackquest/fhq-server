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
    bool create_container(std::string name, std::string &error);
    bool find_container(std::string name);
    bool remove_container(std::string name, std::string error);
    bool send_post_request(std::string address, std::string settings, std::string & response, std::string & error);
    bool send_put_request(std::string address, std::string settings, std::string & response, std::string & error);
    bool send_get_request(std::string address, std::string & response, std::string & error);
    bool send_delete_request(std::string address, std::string &response, std::string & error);

    std::list<std::string> registry_names();
    LXDContainer * get_container(std::string name);
    std::string lastError();

private:
    std::map<std::string, LXDContainer *> containers_map;
    std::string path_dir_lxc_ssl;
    std::string lxd_address;
    std::string m_sLastError;
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

