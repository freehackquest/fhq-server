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

    void initSettings();

    LXDContainer create_container(std::string name);
    LXDContainer find_container(std::string name);
    bool remove_container(std::string name);


private:
    std::map<std::string, LXDContainer *> containers_map;
    bool pull_container_names();
    std::list<std::string> names;
    std::string path_dir_lxc_ssl;
    std::string lxd_address;
    QString TAG;
};

#endif // EMPLOY_ORCHESTRA

