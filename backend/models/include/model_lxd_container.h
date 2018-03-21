#ifndef MODEL_LXD_CONTAINER
#define MODEL_LXD_CONTAINER

#include <QString>
#include <string>
#include <curl/curl.h>
#include <QJsonObject>
#include <QFile>
#include <employ_settings.h>

class LXDContainer {
    public:
        LXDContainer(QJsonObject jsonData);
        QString name;
        QString status;
        QString IPv4;
        QString error;

        QJsonObject state();
        bool create(std::string name);
        bool start();
        bool stop();
        bool remove();
        bool read_file(QFile & file);
        QString exec(QString);


    private:
        QString lxd_address;
        QString path_dir_lxc_ssl;
        std::string response;
};

#endif // MODEL_LXD_CONTAINER

