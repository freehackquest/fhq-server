#ifndef MODEL_LXD_CONTAINER
#define MODEL_LXD_CONTAINER

#include <string>
#include <curl/curl.h>
#include <QFile>
#include <employ_settings.h>
#include <json.hpp>

class LXDContainer {
    public:
        LXDContainer(std::string name_of_container);
        std::string get_name();
        std::string get_status();
        std::string get_IPv4();
        std::string get_error();
        std::string set_prefix();
        std::string full_name();

        bool state(nlohmann::json &jsonState);
        bool create();
        bool start();
        bool stop();
        bool remove();
        bool read_file(std::string path, QFile & file);
        std::string exec(std::string command);


    private:
        std::string name;
        std::string status;
        std::string IPv4;
        std::string m_sError;
        std::string prefix;
        std::string TAG = "EmployOrchestraContainer";

        bool check_response(nlohmann::json res_json);
        bool check_async_response(nlohmann::json operation_json);
};

#endif // MODEL_LXD_CONTAINER

