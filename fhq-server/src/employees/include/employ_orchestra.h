#ifndef EMPLOY_ORCHESTRA
#define EMPLOY_ORCHESTRA

#include <employees.h>
#include <model_lxd_container.h>

#include <list>
#include <string>
#include <map>
#include <curl/curl.h>


class EmployOrchestra : public IEmploy {
public:
    static std::string getEmployName() { return "EmployOrchestra"; }
    virtual void test();

    bool initSettings();

    LXDContainer * get_container(std::string name);
    bool create_container(std::string name, std::string &error);
    bool find_container(std::string name);
    bool remove_container(std::string name);
    bool send_post_request(std::string address, std::string settings, std::string & response, std::string & error);
    bool send_put_request(std::string address, std::string settings, std::string & response, std::string & error);
    bool send_get_request(std::string address, std::string & response, std::string & error);

private:
    std::map<std::string, LXDContainer *> containers_map;
    std::list<std::string> names;
    std::string path_dir_lxc_ssl;
    std::string lxd_address;
    std::string TAG;

    bool pull_container_names();
    bool check_response(nlohmann::json res_json, std::string error);
    bool check_async_response(nlohmann::json operation_json, std::string error);
    std::string extract_keys(std::map<std::string, LXDContainer *> const& input_map);
};

#endif // EMPLOY_ORCHESTRA

