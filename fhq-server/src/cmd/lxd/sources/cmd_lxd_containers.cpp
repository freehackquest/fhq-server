#include <cmd_lxd_containers.h>
#include <employ_orchestra.h>
#include <model_lxd_container.h>

#include <string>
#include <QJsonValue>

CmdLXDContainersHandler::CmdLXDContainersHandler(){

    //Не забудь поменять на false
    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Container name"));
    m_vInputs.push_back(CmdInputDef("action").string_().required().description("Actions: create, start, stop and delete container"));
}

// ---------------------------------------------------------------------

std::string CmdLXDContainersHandler::cmd(){
    return "lxd_containers";
}
// ---------------------------------------------------------------------

const ModelCommandAccess & CmdLXDContainersHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdLXDContainersHandler::inputs(){
    return m_vInputs;
}

std::string CmdLXDContainersHandler::description(){
    return "Any actions with the container. Actions: create, start, stop and delete container";
}

// ---------------------------------------------------------------------

void CmdLXDContainersHandler::handle(ModelRequest *pRequest){
    using namespace std;
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    string name = jsonRequest["name"].toString().trimmed().toStdString();

    //TO DO
    //Action switch
    std::string action = jsonRequest["action"].toString().toStdString();
    if (action == "create")
        create_container(name, jsonResponse);

    if (action == "start")
        start_container(name, jsonResponse);

    if (action == "stop")
        stop_container(name, jsonResponse);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


void CmdLXDContainersHandler::create_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    pOrchestra->initSettings();
    LXDContainer container = pOrchestra->create_container(name);
    if(!(container.get_error() == ""))
        jsonResponse["error"] = QJsonValue(QString::fromStdString(container.get_error()));
}


void CmdLXDContainersHandler::start_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    pOrchestra->initSettings();
    LXDContainer container = pOrchestra->find_container(name);
    if (!container.start())
        jsonResponse["error"] = QJsonValue(QString::fromStdString(container.get_error()));
}


void CmdLXDContainersHandler::stop_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    pOrchestra->initSettings();
    LXDContainer container = pOrchestra->find_container(name);
    if (!container.stop())
        jsonResponse["error"] = QJsonValue(QString::fromStdString(container.get_error()));
}
