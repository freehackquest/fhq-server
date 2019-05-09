#include <cmd_handlers_lxd.h>
#include <utils_logger.h>
#include <runtasks.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_orchestra.h>
#include <vector>
#include <algorithm>
#include <include/cmd_handlers_lxd.h>
#include <QtCore/QDir>
#include <quazip.h>
#include <quazipfile.h>
#include <QtCore/QBuffer>


/*********************************************
 * Any actions with the container. Actions: create, start, stop and delete container
**********************************************/

CmdHandlerLXDContainers::CmdHandlerLXDContainers()
        : CmdHandlerBase("lxd_containers",
                         "Any actions with the container. Actions: create, start, stop and delete container") {

    TAG = "LXD_HANDLER";

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("name", "Container name");
    requireStringParam("action", "Actions: create, start, stop and delete container"); // TODO validator
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
    const std::vector<std::string> actions = {"create", "start", "stop", "delete"};

    if (std::find(actions.begin(), actions.end(), action) == actions.end()) {
        sError = "Non-existent operation. Possible operations: create, start, stop, delete.";
        nErrorCode = 404;
    }

    if (!sError.empty()) {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
    }
    if (action == "create") {
        RunTasks::LXDAsyncOperation(CmdHandlerLXDContainers::create_container, name, cmd(), pRequest);
    }
    if (action == "start") {
        RunTasks::LXDAsyncOperation(CmdHandlerLXDContainers::start_container, name, cmd(), pRequest);
    }
    if (action == "stop") {
        RunTasks::LXDAsyncOperation(CmdHandlerLXDContainers::stop_container, name, cmd(), pRequest);
    }
    if (action == "delete") {
        RunTasks::LXDAsyncOperation(CmdHandlerLXDContainers::delete_container, name, cmd(), pRequest);
    }
}

// ---------------------------------------------------------------------

void CmdHandlerLXDContainers::create_container(const std::string &name, std::string &sError, int &nErrorCode) {
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

void CmdHandlerLXDContainers::start_container(const std::string &name, std::string &sError, int &nErrorCode) {
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

void CmdHandlerLXDContainers::stop_container(const std::string &name, std::string &sError, int &nErrorCode) {
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

void CmdHandlerLXDContainers::delete_container(const std::string &name, std::string &sError, int &nErrorCode) {
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

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("name", "Container name");
    optionalStringParam("get", "Requested information");

}

// ---------------------------------------------------------------------

void CmdHandlerLXDInfo::handle(ModelRequest *pRequest) {
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;
    std::string sError;
    int nErrorCode;
    std::string sName = jsonRequest["name"].toString().trimmed().toStdString();
    nlohmann::json jsonState;

    if (get_state(sName, sError, nErrorCode, jsonState)) {
        jsonResponse["state"] = jsonState.dump();
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

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);
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

// ---------------------------------------------------------------------

CmdHandlerLXDExec::CmdHandlerLXDExec()
        : CmdHandlerBase("lxd_exec", "Exec command in the container with name.") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireStringParam("name", "Container name");
    requireStringParam("command", "Name of execution command"); // TODO validator
}


void CmdHandlerLXDExec::handle(ModelRequest *pRequest) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()) {
        pRequest->sendMessageError(cmd(), Error(500, pOrchestra->lastError()));
        return;
    }
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;
    std::string sError;
    int nErrorCode = 500;
    std::string name = jsonRequest["name"].toString().toStdString();
    std::string command = jsonRequest["command"].toString().toStdString();
    std::string sOutput;

    if (exec_command(name, command, sError, nErrorCode, sOutput)) {
        jsonResponse["container"] = name;
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
        return;
    }
    pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
}

bool CmdHandlerLXDExec::exec_command(const std::string &sName, const std::string &sCommand, std::string &sError,
                                     int &nErrorCode, std::string &sOutput) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();

    if (!pOrchestra->initConnection()) {
        sError = "Can\'t connect to LXD server";
        nErrorCode = 502;
        return false;
    }

    LXDContainer *pContainer;
    if (!pOrchestra->find_container(sName, pContainer)) {
        sError = "Not found container " + sName;
        nErrorCode = 404;
        return false;
    }

    if (!pContainer->exec(sCommand)) {
        sError = pContainer->get_error();
        nErrorCode = 500;
        return false;
    }

    sOutput = pContainer->get_result();
    return true;
}

// ---------------------------------------------------------------------

CmdHandlerLXDFile::CmdHandlerLXDFile()
        : CmdHandlerBase("lxd_file", "Pull, push, delete file inside the container.") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireStringParam("name", "Container name");
    requireStringParam("action", "Action with files: pull, push or delete"); // TODO validator
    requireStringParam("path", "Path to file inside the container");
}

void CmdHandlerLXDFile::handle(ModelRequest *pRequest) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()) {
        pRequest->sendMessageError(cmd(), Error(500, pOrchestra->lastError()));
        return;
    }
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;
    std::string sError;
    int nErrorCode = 500;
    std::string name = jsonRequest["name"].toString().toStdString();
    std::string action = jsonRequest["action"].toString().toStdString();
    std::string path = jsonRequest["path"].toString().toStdString();
    std::string sb64File;

    const std::vector<std::string> actions = {"pull", "push", "delete"};
    if (std::find(actions.begin(), actions.end(), action) == actions.end()) {
        sError = "Non-existent operation. Possible operations: pull, push, delete.";
        nErrorCode = 404;
    }

    if (!sError.empty()) {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
        return;
    }

    LXDContainer *pContainer;
    if (!pOrchestra->find_container(name, pContainer)) {
        sError = "Not found container " + name;
        nErrorCode = 404;
    }

    bool isDirectory = false;

    if (path.back() == '/') {
        isDirectory = true;
        sError = path + " is the directory!";
        nErrorCode = 400;
    }

    if (!sError.empty()) {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
        return;
    }

    if (!isDirectory && action == "pull") {
        pull_file(pContainer, path, sb64File, sError, nErrorCode, isDirectory);
    } else if (action == "push") {
        sb64File = jsonRequest["file_base64"].toString().toStdString();
        push_file(pContainer, path, sb64File, sError, nErrorCode);
    }

    if (sError.empty() && !isDirectory) {
        jsonResponse["container"] = name;
        jsonResponse["path"] = path;

        if (action == "pull") {
            jsonResponse["file_base64"] = sb64File;
        } else if (action == "push") {
            jsonResponse["status"] = "Success";
        }
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
    } else {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
    }
}

void CmdHandlerLXDFile::pull_file(LXDContainer *pContainer, const std::string &sPath, std::string &sb64File,
                                  std::string &sError, int &nErrorCode, bool &isDirectory) {
    std::string sRawData;

    if (!pContainer->read_file(sPath, sRawData)) {
        sError = pContainer->get_error();
        nErrorCode = 500;
    }

    if (nlohmann::json::accept(std::begin(sRawData), std::end(sRawData))) {
        auto jsonResponse = nlohmann::json::parse(sRawData);
        if (jsonResponse.find("metadata") != jsonResponse.end() && jsonResponse.find("status") != jsonResponse.end()) {
            isDirectory = true;
            sError = sPath + " is the directory!";
            nErrorCode = 400;
            return;
        }
    }
    sb64File = QByteArray::fromStdString(sRawData).toBase64().toStdString();
}

bool CmdHandlerLXDFile::push_file(LXDContainer *pContainer, const std::string &sPath, const std::string &sb64File,
                                  std::string &sError, int &nErrorCode) {
    QByteArray RawData = QByteArray::fromBase64(QByteArray::fromStdString(sb64File));
    return pContainer->push_file(sPath, RawData.toStdString());
}

CmdHandlerLXDOpenPort::CmdHandlerLXDOpenPort()
        : CmdHandlerBase("lxd_open_port", "Opens the container port.") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireStringParam("name", "Container name");
    requireIntegerParam("port", "Number container port");
    requireStringParam("protocol", "Protocol");
}

void CmdHandlerLXDOpenPort::handle(ModelRequest *pRequest) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()) {
        pRequest->sendMessageError(cmd(), Error(500, pOrchestra->lastError()));
        return;
    }
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;
    std::string sError;
    int nErrorCode = 500;
    const std::string sName = jsonRequest["name"].toString().toStdString();
    const int nPort = jsonRequest["port"].toInt();
    const std::string sProto = jsonRequest["protocol"].toString().toStdString();

    if (!is_port_valide(sProto, nPort, sError, nErrorCode)) {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
        return;
    }

    LXDContainer *pContainer;
    if (!pOrchestra->find_container(sName, pContainer)) {
        sError = "Not found container " + sName;
        std::cout << sName << std::endl;
        nErrorCode = 404;
    }

    if (!sError.empty()) {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
        return;
    }

    if (!pContainer->open_port(nPort, sProto)) {
        sError = pContainer->get_error();
    }

    if (sError.empty()) {
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
    } else {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
    }
}

bool CmdHandlerLXDOpenPort::is_port_valide(const std::string &sProto, const int &nPort, std::string &sError,
                                           int &nErrorCode) {

    if (!(sProto == "tcp" || sProto == "udp")) {
        sError = "Only tcp or udp protocols. Not " + sProto;
        nErrorCode = 400;
        return false;
    }

    if (nPort >= 49152) {
        sError = "Port " + std::to_string(nPort) + " is reserved.";
        nErrorCode = 400;
        return false;
    } else if (nPort <= 0) {
        sError = "Port number must be a positive integer.";
        nErrorCode = 400;
        return false;
    }

    return true;
}


CmdHandlerLXDImportService::CmdHandlerLXDImportService()
        : CmdHandlerBase("lxd_import_container", "Import container from json configuration.") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireStringParam("config", "Container's configuration in json dumped string.");
}

void CmdHandlerLXDImportService::handle(ModelRequest *pRequest) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()) {
        pRequest->sendMessageError(cmd(), Error(500, pOrchestra->lastError()));
        return;
    }
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;
    std::string sError;
    int nErrorCode = 500;
    std::string sName = jsonRequest["name"].toString().toStdString();
    const std::string sConfig = jsonRequest["config"].toString().toStdString();

    if (!nlohmann::json::accept(sConfig)) {
        pRequest->sendMessageError(cmd(), Error(400, "Json string isn't valid."));
    }
    auto jsonConfig = nlohmann::json::parse(sConfig);

    if (sName.empty()) {
        if (jsonConfig.find("name") != jsonConfig.end()) {
            sName = jsonConfig["name"];
        } else {
            pRequest->sendMessageError(cmd(), Error(400, "Container name not found."));
        }

    }

    LXDContainer *pContainer;
    if (pOrchestra->find_container(sName, pContainer)) {
        sError = "Container " + sName + " is already created.";
        std::cout << sError << std::endl;
        nErrorCode = 400;
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
    }

    ServiceConfig serviceReq = ServiceConfig(jsonConfig);

    if (!pOrchestra->create_service(serviceReq, sError)) {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
    }

    if (sError.empty()) {
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
    } else {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
    }
}


CmdHandlerLXDImportServiceFromZip::CmdHandlerLXDImportServiceFromZip()
        : CmdHandlerBase("lxd_import_service_from_zip", "Import Service from zip.") {

    setAccessUnauthorized(true);
    setAccessUser(false);
    setAccessAdmin(true);

    requireStringParam("zip_file", "Service's configuration in Base64 zip archive.");
    requireStringParam("name", "Service's name.");
}

void CmdHandlerLXDImportServiceFromZip::handle(ModelRequest *pRequest) {
    auto *pOrchestra = findEmploy<EmployOrchestra>();
    if (!pOrchestra->initConnection()) {
        pRequest->sendMessageError(cmd(), Error(500, pOrchestra->lastError()));
        return;
    }
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;
    std::string sError;
    int nErrorCode = 500;
    const std::string qsB64Zip = jsonRequest["zip_file"].toString().toStdString();
    const std::string name = jsonRequest["name"].toString().toStdString();
    std::string sConfig;

    QByteArray qbaZip = QByteArray::fromBase64(QByteArray::fromStdString(qsB64Zip));
    QBuffer buffer(&qbaZip);
    buffer.open(QIODevice::ReadOnly);
    QuaZip zip(&buffer);
    QuaZipFile file(&zip);

    if (zip.open(QuaZip::mdUnzip)) {

        if (zip.getZipError() != UNZ_OK) {
            pRequest->sendMessageError(cmd(), Error(400, "Cant unzip " + std::to_string(zip.getZipError())));
            return;
        } else {
            for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
                if (zip.getCurrentFileName().endsWith("service.json")) {
                    file.open(QIODevice::ReadOnly);
                    sConfig = file.readAll().toStdString();
                    file.close();
                    break;
                }
            }
        }
    }

    if (sConfig.empty()) {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, "Not found service.json in zip archive."));
        return;
    }

    if (!nlohmann::json::accept(sConfig)) {
        pRequest->sendMessageError(cmd(), Error(400, "Service json file isn't valid."));
    }
    auto jsonConfig = nlohmann::json::parse(sConfig);
    const ServiceConfig config = ServiceConfig(jsonConfig);
    ServiceLXD service = ServiceLXD(config);
    service.create_container();
    LXDContainer *container = service.get_container();

    if (!container->exec("mkdir -p /root/service")) {
        sError = "Cant create service directory.";
    }

    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        std::string file_name = zip.getCurrentFileName().section('/', -1).toStdString();
        if (!file_name.empty() && file_name != "service.json") {
            file.open(QIODevice::ReadOnly);
            if (!container->push_file("/root/service/" + file_name, file.readAll().toStdString())) {
                pRequest->sendMessageError(cmd(), Error(nErrorCode, container->get_error()));
                file.close();
                return;
            }
            file.close();
        }
    }

    zip.close();

    if (sError.empty()) {
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
    } else {
        pRequest->sendMessageError(cmd(), Error(nErrorCode, sError));
    }
}