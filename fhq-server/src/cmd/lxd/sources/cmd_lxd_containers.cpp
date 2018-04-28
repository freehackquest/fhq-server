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

    TAG = "LXD_HANDLER";
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

    if (action == "delete")
        delete_container(name, jsonResponse);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


void CmdLXDContainersHandler::create_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    //Переместить в Orchestra
    if (!pOrchestra->initSettings())
        return;

    std::string error;
    if (pOrchestra->find_container(name)){
        jsonResponse["error"] = QJsonValue("Container already exists");
        Log::err(TAG, "Container already exists");
        return;
    }
    if (!pOrchestra->create_container(name, error)){
        jsonResponse["error"] = QJsonValue(QString::fromStdString(error));
        Log::err(TAG, "Can\'t create container");
        return;
    }
}


void CmdLXDContainersHandler::start_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initSettings())
        return;

    LXDContainer * container;
    if (!pOrchestra->find_container(name)){
       jsonResponse["error"] = QJsonValue("Can\'t find container " + QString::fromStdString(name));
       Log::err(TAG, "Can\'t find container" + name);
       return;
    }

    container = pOrchestra->get_container(name);

    if (!container->start())
        jsonResponse["error"] = QJsonValue(QString::fromStdString(container->get_error()));
}


void CmdLXDContainersHandler::stop_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initSettings())
        return;

    LXDContainer * container;
    if (!pOrchestra->find_container(name)){
       jsonResponse["error"] = QJsonValue("Can\'t find container");
       return;
    }

    container = pOrchestra->get_container(name);

    if (!container->stop())
        jsonResponse["error"] = QJsonValue(QString::fromStdString(container->get_error()));
}

void CmdLXDContainersHandler::delete_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initSettings())
        return;

    if (!pOrchestra->find_container(name)){
       jsonResponse["error"] = QJsonValue("Can\'t find container");
       return;
    }

    std::string error;
    if (!pOrchestra->remove_container(name, error))
        jsonResponse["error"] = QJsonValue(QString::fromStdString(error));
}
