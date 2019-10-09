#ifndef MODEL_LXD_CONTAINER
#define MODEL_LXD_CONTAINER

#include <string>
#include <curl/curl.h>
#include <QFile>
#include <json.hpp>


struct ServiceConfig {
    std::string name;
    std::string type;
    std::string game;
    std::string author;
    std::string version;
    std::string port_proto;

    int port_number;
    bool start;
    bool build;

    explicit ServiceConfig(nlohmann::json jsonConfig);
};


class LXDContainer {
public:
    explicit LXDContainer(const std::string &name_of_container);
    std::string get_name() const;
    std::string get_IPv4() const;
    std::string get_port();
    std::string get_error() const;
    std::string get_result() const;
    std::string full_name() const;

    bool get_state(nlohmann::json &jsonState);
    std::string get_status();
    bool create();
    bool start();
    bool stop();
    bool remove();
    bool read_file(const std::string &sPath, std::string &sRawData);
    bool push_file(const std::string &sPath, const std::string &sRawData);
    bool open_port(const std::string &sPort, const std::string &sProto);
    bool open_port(const int &nPort, const std::string &sProto);
    bool exec(const std::string &sCommand);

    static std::vector<std::string> split(const std::string &str);


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
    explicit ServiceLXD(const ServiceConfig &reqService);

    bool create_container();
    bool build();
    bool start();
    bool stop();

    std::string get_error();
    LXDContainer *get_container();

private:
    ServiceConfig m_configService;
    LXDContainer *m_Container;

    std::string m_sName;
    std::string m_sError;
    int m_nError = 500;
    std::string m_sResult;
    const std::string TAG = "EmployOrchestraService";
};

#endif // MODEL_LXD_CONTAINER

