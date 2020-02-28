#include "wsjcpp_light_web_http_handler_web_folder.h"
#include <wsjcpp_core.h>

// ----------------------------------------------------------------------

WSJCppLightWebHttpHandlerWebFolder::WSJCppLightWebHttpHandlerWebFolder(const std::string &sPrefixPath, const std::string &sWebFolder)
: WSJCppLightWebHttpHandlerBase("web-folder") {

    TAG = "WSJCppLightWebHttpHandlerWebFolder";
    m_sPrefixPath = sPrefixPath;
    m_sWebFolder = sWebFolder;
}

// ----------------------------------------------------------------------

bool WSJCppLightWebHttpHandlerWebFolder::canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // WSJCppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->getRequestPath();
    
    if (m_sPrefixPath.length() > sRequestPath.length()) {
        return false;
    }

    std::string sPrefixPath = sRequestPath.substr(0, m_sPrefixPath.length()); 
    if (sPrefixPath != m_sPrefixPath) {
        return false;
    }

    if (!WSJCppCore::dirExists(m_sWebFolder)) {
        WSJCppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
    }
    return true;
}

// ----------------------------------------------------------------------

bool WSJCppLightWebHttpHandlerWebFolder::handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WSJCppLog::warn(_tag, pRequest->requestPath());
    if (sRequestPath == "/") {
        sRequestPath = "/index.html";
    }
    std::string sFilePath = m_sWebFolder + sRequestPath;
    if (WSJCppCore::fileExists(sFilePath)) {
        WSJCppLightWebHttpResponse resp(pRequest->getSockFd());
        resp.cacheSec(60).ok().sendFile(sFilePath);
    } else {
        WSJCppLightWebHttpResponse resp(pRequest->getSockFd());
        resp.noCache().notFound().sendEmpty();
    }
    return true;
}