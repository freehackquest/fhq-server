#include "http_handler_web_user_folder.h"
#include <fallen.h>

// ----------------------------------------------------------------------

HttpHandlerWebUserFolder::HttpHandlerWebUserFolder(const std::string &sWebFolder)
    : WsjcppLightWebHttpHandlerBase("web-user-folder") {

    TAG = "HttpHandlerWebUserFolder";
    m_sWebFolder = sWebFolder;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebUserFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->getRequestPath();
    
    if (sRequestPath == "") {
        sRequestPath = "/";
        WsjcppLog::warn(_tag, "Request path is empty");
    }

    if (sRequestPath == "/") {
        sRequestPath = "/index.html";
    }

    if (!WsjcppCore::dirExists(m_sWebFolder)) {
        WsjcppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
    }
    return true;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebUserFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WsjcppLog::warn(_tag, pRequest->requestPath());
    
    if (sRequestPath == "") {
        sRequestPath = "/";
    }
    
    std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
    if (WsjcppCore::fileExists(sFilePath)) {
        WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
        resp.cacheSec(60).ok().sendFile(sFilePath);
    } else {
        std::string sFilePath = m_sWebFolder + "/index.html";
        WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
        resp.cacheSec(60).ok().sendFile(sFilePath);    
    }
    return true;
}
