#include "http_handler_web_admin_folder.h"
#include <fallen.h>

// ----------------------------------------------------------------------

HttpHandlerWebAdminFolder::HttpHandlerWebAdminFolder(const std::string &sWebFolder)
    : WsjcppLightWebHttpHandlerBase("web-admin-folder") {

    TAG = "HttpHandlerWebAdminFolder";
    m_sWebFolder = sWebFolder;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebAdminFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->getRequestPath();
    
    if (sRequestPath == "") {
        WsjcppLog::err(_tag, "Request path is empty");
        return false;
    }

    if (sRequestPath == "/admin") {
        sRequestPath = "/admin/";
    }

    if (sRequestPath == "/admin/") {
        sRequestPath = "/admin/index.html";
    }

    std::string s = "/admin/";
    if (sRequestPath.rfind("/admin/", 0) != 0) {
        return false;
    }

    if (!WsjcppCore::dirExists(m_sWebFolder)) {
        WsjcppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
    }
    sRequestPath = sRequestPath.substr(6); // remove /admin

    std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
    // WsjcppLog::warn(_tag, "Response Resources " + sFilePath);
    if (!WsjcppCore::fileExists(sFilePath)) { // TODO check the file exists not dir
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebAdminFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WsjcppLog::warn(_tag, pRequest->requestPath());
    
    if (sRequestPath == "/admin") {
        sRequestPath = "/admin/";
    }

    if (sRequestPath == "/admin/") {
        sRequestPath = "/admin/index.html";
    }

    sRequestPath = sRequestPath.substr(6); // remove /admin
    
    std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
    if (!WsjcppCore::fileExists(sFilePath)) {
        // std::string sResPath = "html" + sRequestPath;
        /*if (ResourcesManager::has(sResPath)) {
            // WsjcppLog::warn(_tag, "Response Resources " + sResPath);
            // ResourceFile *pFile = ResourcesManager::get(sResPath);
            // pRequest->responseBuffer(sResPath, pFile->buffer(), pFile->bufferSize());
            return true;
        }*/
        return false;
    }

    // TODO 1. if file exists and last date change more that in cache so need update
    // TODO 2. if file not exists but in in resources - response them
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
    // WsjcppLog::warn(_tag, "Response File " + sFilePath);
    resp.cacheSec(60).ok().sendFile(sFilePath);
    return true;
}
