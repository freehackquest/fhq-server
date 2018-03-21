#ifndef EMPLOY_ORCHESTRA
#define EMPLOY_ORCHESTRA

#include <employees.h>
#include <model_lxd_container.h>

#include <QString>
#include <string>
#include <curl/curl.h>
#include <QMap>


class EmployOrchestra : public IEmploy {
public:
    static std::string getEmployName() { return "EmployOrchestra"; }
    virtual void test();

    void initSettings();

    LXDContainer create_container(std::string name);
    LXDContainer find_container(std::string name);
    bool remove_container(std::string name);


private:
    QMap<std::string, LXDContainer> containers_map;
    bool pull_container_names();
    QStringList names;
    QString path_dir_lxc_ssl;
    QString lxd_address;
    QString TAG;
};

#endif // EMPLOY_ORCHESTRA

