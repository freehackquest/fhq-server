#ifndef MODEL_LXD_CONTAINER
#define MODEL_LXD_CONTAINER

#include <string>
#include <curl/curl.h>
#include <QFile>
#include <employ_settings.h>
#include <json.hpp>

class LXDContainer {
public:
    explicit LXDContainer(const std::string &name_of_container);
    std::string get_name() const;
    std::string get_status() const;
    std::string get_IPv4() const;
    std::string get_port();
    std::string get_error() const;
    std::string get_result() const;
    std::string full_name() const;

    bool state(nlohmann::json &jsonState);
    bool create();
    bool start();
    bool stop();
    bool remove();
    bool read_file(std::string path, QFile &file);
    bool exec(const std::string &sCommand);

    std::vector<std::string> split(const std::string& str);


private:
    std::string name;
    std::string status;
    std::string IPv4;
    std::string prefix;
    std::string m_sError;
    std::string m_sResult;
    std::string m_sPort;
    const std::string TAG = "EmployOrchestraContainer";
};

#endif // MODEL_LXD_CONTAINER

