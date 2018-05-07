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
    std::string action = jsonRequest["action"].toString().toStdString();

    std::string sError;
    int nErrorCode;

    if (action == "create")
        create_container(name, sError, nErrorCode);

    if (action == "start")
        start_container(name, sError, nErrorCode);

    if (action == "stop")
        stop_container(name, sError, nErrorCode);

    if (action == "delete")
        delete_container(name, sError, nErrorCode);

    if (sError == "")
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
    else
        pRequest->sendMessageError(cmd(), Error(nErrorCode, QString::fromStdString(sError)));
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::create_container(std::string name, std::string &sError, int &nErrorCode){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    //Переместить в Orchestra
    if (!pOrchestra->initConnection()){
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return;
    }

    if (pOrchestra->find_container(name)){
        sError = "Container " + name + " already exists";
        nErrorCode = 409;
        Log::err(TAG, sError);
        return;
    }
    if (!pOrchestra->create_container(name, sError)){
        sError = "Can\'t create container";
        nErrorCode = 500;
        Log::err(TAG, sError);
        return;
    }
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::start_container(std::string name, std::string &sError, int &nErrorCode){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()){
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return;
    }

    LXDContainer * pContainer;
    if (!pOrchestra->find_container(name)){
       sError = "Not found container " + name;
       nErrorCode = 404;
       Log::err(TAG, sError);
       return;
    }

    pContainer = pOrchestra->get_container(name);

    if (!pContainer->start()){
        sError = pContainer->get_error();
        nErrorCode = 500;
    }
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::stop_container(std::string name, std::string &sError, int &nErrorCode){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()){
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return;
    }

    LXDContainer * pContainer;
    if (!pOrchestra->find_container(name)){
        sError = "Not found container " + name;
        nErrorCode = 404;
        Log::err(TAG, sError);
        return;
    }

    pContainer = pOrchestra->get_container(name);

    if (!pContainer->stop()){
        sError = pContainer->get_error();
        nErrorCode = 500;
    }
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::delete_container(std::string name, std::string &sError, int &nErrorCode){
    EmployOrchestra *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()){
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return;
    }

    if (!pOrchestra->find_container(name)){
        sError = "Not found container " + name;
        nErrorCode = 404;
        Log::err(TAG, sError);
       return;
    }

    if (!pOrchestra->remove_container(name, sError))
        nErrorCode = 500;
        return;
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

