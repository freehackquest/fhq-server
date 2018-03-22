#ifndef MODEL_LXD_CONTAINER
#define MODEL_LXD_CONTAINER

#include <string>
#include <curl/curl.h>
#include <QJsonObject>
#include <QFile>
#include <employ_settings.h>

class LXDContainer {
    public:
        LXDContainer(QJsonObject jsonData);
        std::string name;
        std::string status;
        std::string IPv4;
        std::string error;

        QJsonObject state();
        bool create(std::string name);
        bool start();
        bool stop();
        bool remove();
        bool read_file(QFile & file);
        std::string exec(std::string);


    private:
        std::string lxd_address;
        std::string path_dir_lxc_ssl;
        std::string response;
};

#endif // MODEL_LXD_CONTAINER

