#include "http_handler_web_admin_folder.h"
#include <fallen.h>

// ----------------------------------------------------------------------

HttpHandlerWebAdminFolder::HttpHandlerWebAdminFolder(const std::string &sWebFolder)
    : LightHttpHandlerBase("web-admin-folder") {

    TAG = "HttpHandlerWebAdminFolder";
    m_sWebFolder = sWebFolder;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebAdminFolder::canHandle(const std::string &sWorkerId, LightHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // Log::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->requestPath();
    
    if (sRequestPath == "") {
        Log::err(_tag, "Request path is empty");
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

    if (!Fallen::dirExists(m_sWebFolder)) {
        Log::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
    }
    sRequestPath = sRequestPath.substr(6); // remove /admin

    std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
    // Log::warn(_tag, "Response Resources " + sFilePath);
    if (!Fallen::fileExists(sFilePath)) { // TODO check the file exists not dir
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebAdminFolder::handle(const std::string &sWorkerId, LightHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->requestPath();
    // Log::warn(_tag, pRequest->requestPath());
    
    if (sRequestPath == "/admin") {
        sRequestPath = "/admin/";
    }

    if (sRequestPath == "/admin/") {
        sRequestPath = "/admin/index.html";
    }

    sRequestPath = sRequestPath.substr(6); // remove /admin
    
    std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
    if (!Fallen::fileExists(sFilePath)) {
        // std::string sResPath = "html" + sRequestPath;
        /*if (ResourcesManager::has(sResPath)) {
            // Log::warn(_tag, "Response Resources " + sResPath);
            // ResourceFile *pFile = ResourcesManager::get(sResPath);
            // pRequest->responseBuffer(sResPath, pFile->buffer(), pFile->bufferSize());
            return true;
        }*/
        return false;
    }

    // TODO 1. if file exists and last date change more that in cache so need update
    // TODO 2. if file not exists but in in resources - response them
    LightHttpResponse resp(pRequest->sockFd());
    // Log::warn(_tag, "Response File " + sFilePath);
    resp.cacheSec(60).ok().sendFile(sFilePath);
    return true;
}
