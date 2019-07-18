#include "http_handler_web_user_folder.h"
#include <fallen.h>

// ----------------------------------------------------------------------

HttpHandlerWebUserFolder::HttpHandlerWebUserFolder(const std::string &sWebFolder)
    : LightHttpHandlerBase("web-user-folder") {

    TAG = "HttpHandlerWebUserFolder";
    m_sWebFolder = sWebFolder;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebUserFolder::canHandle(const std::string &sWorkerId, LightHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // Log::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->requestPath();
    
    if (sRequestPath == "") {
        sRequestPath = "/";
        Log::warn(_tag, "Request path is empty");
    }

    if (sRequestPath == "/") {
        sRequestPath = "/index.html";
    }

    if (!Fallen::dirExists(m_sWebFolder)) {
        Log::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
    }
    return true;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebUserFolder::handle(const std::string &sWorkerId, LightHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->requestPath();
    // Log::warn(_tag, pRequest->requestPath());
    
    if (sRequestPath == "") {
        sRequestPath = "/";
    }
    
    std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
    if (Fallen::fileExists(sFilePath)) {
        LightHttpResponse resp(pRequest->sockFd());
        resp.cacheSec(60).ok().sendFile(sFilePath);
    } else {
        std::string sFilePath = m_sWebFolder + "/index.html";
        LightHttpResponse resp(pRequest->sockFd());
        resp.cacheSec(60).ok().sendFile(sFilePath);    
    }
    return true;
}
