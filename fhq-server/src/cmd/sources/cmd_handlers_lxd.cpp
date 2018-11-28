#include <cmd_handlers_lxd.h>
#include <utils_logger.h>
#include <runtasks.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_orchestra.h>
#include <vector>
#include <algorithm>

/*********************************************
 * Any actions with the container. Actions: create, start, stop and delete container
**********************************************/

CmdHandlerLXDContainers::CmdHandlerLXDContainers()
        : CmdHandlerBase("lxd_containers",
                         "Any actions with the container. Actions: create, start, stop and delete container") {

    TAG = "LXD_HANDLER";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Container name"));
    m_vInputs.push_back(CmdInputDef("action").string_().required().description(
            "Actions: create, start, stop and delete container"));
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::handle(ModelRequest *pRequest) {
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
    auto *pDatabase = findEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());

    std::string name = jsonRequest["name"].toString().trimmed().toStdString();
    std::string action = jsonRequest["action"].toString().toStdString();

    std::string sError;
    int nErrorCode = 500;
    std::vector<std::string> actions = {"create", "start", "stop", "delete"};

    if (std::find(actions.begin(), actions.end(), action) == actions.end()) {
        sError = "Non-existent operation. Possible operations: create, start, stop, delete.";
        nErrorCode = 404;
    }

    if (!sError.empty())
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));

    if (action == "create")
        RunTasks::LXDAsyncOperation(CmdHandlerLXDContainers::create_container, name, cmd(), pRequest);

    if (action == "start")
        RunTasks::LXDAsyncOperation(CmdHandlerLXDContainers::start_container, name, cmd(), pRequest);

    if (action == "stop")
        RunTasks::LXDAsyncOperation(CmdHandlerLXDContainers::stop_container, name, cmd(), pRequest);

    if (action == "delete")
        RunTasks::LXDAsyncOperation(CmdHandlerLXDContainers::delete_container, name, cmd(), pRequest);
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::create_container(std::string name, std::string &sError, int &nErrorCode) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    LXDContainer *pContainer;

    //Переместить в Orchestra
    if (!pOrchestra->initConnection()) {
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return;
    }

    if (pOrchestra->find_container(name, pContainer)) {
        sError = "Container " + name + " already exists";
        nErrorCode = 409;
        return;
    }
    if (!pOrchestra->create_container(name, sError)) {
        sError = "Can\'t create container";
        nErrorCode = 500;
        return;
    }
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::start_container(std::string name, std::string &sError, int &nErrorCode) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()) {
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return;
    }

    LXDContainer *pContainer;
    if (!pOrchestra->find_container(name, pContainer)) {
        sError = "Not found container " + name;
        nErrorCode = 404;
        return;
    }

    if (!pContainer->start()) {
        sError = pContainer->get_error();
        nErrorCode = 500;
    }
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::stop_container(std::string name, std::string &sError, int &nErrorCode) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()) {
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return;
    }

    LXDContainer *pContainer;
    if (!pOrchestra->find_container(name, pContainer)) {
        sError = "Not found container " + name;
        nErrorCode = 404;
        return;
    }

    if (!pContainer->stop()) {
        sError = pContainer->get_error();
        nErrorCode = 500;
    }
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::delete_container(std::string name, std::string &sError, int &nErrorCode) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    LXDContainer *pContainer;

    if (!pOrchestra->initConnection()) {
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return;
    }

    if (!pOrchestra->find_container(name, pContainer)) {
        sError = "Not found container " + name;
        nErrorCode = 404;
        return;
    }

    if (!pOrchestra->remove_container(name, sError)) {
        nErrorCode = 500;
        return;
    }
}


/*********************************************
 * Get information about the orhestra, containers.
**********************************************/

CmdHandlerLXDInfo::CmdHandlerLXDInfo()
        : CmdHandlerBase("lxd_info", "Get information about the orhestra, containers.") {

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Container name"));
    m_vInputs.push_back(CmdInputDef("get").string_().optional().description("Requested information"));
}

// ---------------------------------------------------------------------

void CmdHandlerLXDInfo::handle(ModelRequest *pRequest) {
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
    std::string sError;
    int nErrorCode;
    std::string sName = jsonRequest["name"].toString().trimmed().toStdString();
    nlohmann::json jsonState;

    if (get_state(sName, sError, nErrorCode, jsonState)) {
        jsonResponse["state"] = QJsonValue(QString::fromStdString(jsonState.dump()));
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
    } else
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
}

bool CmdHandlerLXDInfo::get_state(std::string sName, std::string &sError, int &nErrorCode, nlohmann::json &jsonState) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()) {
        return false;
    }

    LXDContainer *pContainer;
    if (!pOrchestra->find_container(sName, pContainer)) {
        sError = "Not found container " + sName;
        nErrorCode = 404;
        Log::err(TAG, sError);
        return false;
    }

    if (!pContainer->state(jsonState)) {
        nErrorCode = 500;
        sError = pContainer->get_error();
        return false;
    }
    return true;
}

/*********************************************
 * Get information about all containers.
**********************************************/

CmdHandlerLXDList::CmdHandlerLXDList()
        : CmdHandlerBase("lxd_list", "Get information about all containers.") {

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerLXDList::handle(ModelRequest *pRequest) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()) {
        pRequest->sendMessageError(cmd(), Error(500, pOrchestra->lastError()));
        return;
    }

    std::list<std::string> vList = pOrchestra->registry_names();

    auto jsonNameContainers = nlohmann::json::array();
    for (auto const &i : vList) {
        jsonNameContainers.push_back(std::string(i));
    }

    nlohmann::json jsonResponse;
    jsonResponse["data"] = vList;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

