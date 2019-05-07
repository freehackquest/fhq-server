#ifndef MODEL_LXD_CONTAINER
#define MODEL_LXD_CONTAINER

#include <string>
#include <curl/curl.h>
#include <QFile>
#include <employ_settings.h>
#include <json.hpp>


struct ServiceRequest {
    std::string name;
    std::string type;
    std::string game;
    std::string author;
    std::string version;
    std::string port_proto;

    int port_number;
    bool start;
    bool build;

    explicit ServiceRequest(nlohmann::json jsonConfig);
};


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
    bool read_file(const std::string &sPath, std::string &sRawData);
    bool push_file(const std::string &sPath, const std::string &sRawData);
    bool open_port(const std::string &sPort, const std::string &sProto);
    bool open_port(const int &nPort, const std::string &sProto);
    bool exec(const std::string &sCommand);

    std::vector<std::string> split(const std::string &str);


private:
    std::string name;
    std::string status;
    std::string IPv4;
    const std::string prefix = "fhq-";
    // TODO ADD nErrorCode
    std::string m_sError;
    int m_nError;
    std::string m_sResult;
    std::string m_sPort;
    std::string m_sProtoPort;
    const std::string TAG = "EmployOrchestraContainer";
};


class ServiceLXD {
public:
    explicit ServiceLXD(const ServiceRequest &reqService);

    bool create_container();
    bool load_service();
    bool build();
    bool start();
    bool stop();

    std::string get_error();

private:
    ServiceRequest m_reqService;
    LXDContainer* m_Container;

    std::string m_sName;
    std::string m_sError;
    int m_nError;
    std::string m_sResult;
    const std::string TAG = "EmployOrchestraService";
};

#endif // MODEL_LXD_CONTAINER

