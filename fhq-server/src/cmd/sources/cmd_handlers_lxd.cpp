#include <cmd_handlers_lxd.h>
#include <log.h>
#include <runtasks.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_orchestra.h>

/*********************************************
 * Any actions with the container. Actions: create, start, stop and delete container
**********************************************/

CmdHandlerLXDContainers::CmdHandlerLXDContainers()
	: CmdHandlerBase("lxd_containers", "Any actions with the container. Actions: create, start, stop and delete container"){

	TAG = "LXD_HANDLER";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

	// validation and description input fields
	m_vInputs.push_back(CmdInputDef("name").string_().required().description("Container name"));
    m_vInputs.push_back(CmdInputDef("action").string_().required().description("Actions: create, start, stop and delete container"));
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::handle(ModelRequest *pRequest){
    using namespace std;
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());

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

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::create_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    //Переместить в Orchestra
    if (!pOrchestra->initConnection())
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

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::start_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection())
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

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::stop_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection())
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

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::delete_container(std::string name, QJsonObject &jsonResponse){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()){
        return;
    }

    if (!pOrchestra->find_container(name)){
       jsonResponse["error"] = QJsonValue("Can\'t find container");
       return;
    }

    std::string error;
    if (!pOrchestra->remove_container(name, error))
        jsonResponse["error"] = QJsonValue(QString::fromStdString(error));
}


/*********************************************
 * Get information about the orhestra, containers.
**********************************************/

CmdHandlerLXDInfo::CmdHandlerLXDInfo()
    : CmdHandlerBase("lxd_info", "Get information about the orhestra, containers."){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Container name"));
    m_vInputs.push_back(CmdInputDef("get").string_().required().description("Requested information"));
}

// ---------------------------------------------------------------------

void CmdHandlerLXDInfo::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()){
        return;
	}
        
    // EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    // QSqlDatabase db = *(pDatabase->database());

    std::string name = jsonRequest["name"].toString().trimmed().toStdString();
    std::string error, response;

    //TO DO
    //Action switch
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


/*********************************************
 * Get information about all containers.
**********************************************/

CmdHandlerLXDList::CmdHandlerLXDList()
    : CmdHandlerBase("lxd_list", "Get information about all containers."){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerLXDList::handle(ModelRequest *pRequest){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()){
        pRequest->sendMessageError(cmd(), Error(500, QString(pOrchestra->lastError().c_str())));
        return;
    }

    std::list<std::string> vList = pOrchestra->registry_names();

    auto jsonNameContainers = nlohmann::json::array();
    for (auto const& i : vList) {
        jsonNameContainers.push_back(std::string(i));
    }

    nlohmann::json jsonResponse;
    jsonResponse["data"] = vList;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

