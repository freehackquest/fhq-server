#include "wsjcpp_light_web_http_handler_web_folder.h"
#include <wsjcpp_core.h>

// ----------------------------------------------------------------------

WsjcppLightWebHttpHandlerWebFolder::WsjcppLightWebHttpHandlerWebFolder(const std::string &sPrefixPath, const std::string &sWebFolder)
: WsjcppLightWebHttpHandlerBase("web-folder") {

    TAG = "WsjcppLightWebHttpHandlerWebFolder";
    m_sPrefixPath = WsjcppCore::doNormalizePath(sPrefixPath + "/");
    m_sWebFolder = WsjcppCore::doNormalizePath(sWebFolder + "/");
}

// ----------------------------------------------------------------------

bool WsjcppLightWebHttpHandlerWebFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->getRequestPath();
    
    if (m_sPrefixPath.length() > sRequestPath.length()) {
        return false;
    }

    std::string sPrefixPath = sRequestPath.substr(0, m_sPrefixPath.length()); 
    if (sPrefixPath != m_sPrefixPath) {
        return false;
    }

    if (!WsjcppCore::dirExists(m_sWebFolder)) {
        WsjcppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
    }
    return true;
}

// ----------------------------------------------------------------------

bool WsjcppLightWebHttpHandlerWebFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WsjcppLog::warn(_tag, sRequestPath);
    std::string sRequestPath2 = sRequestPath.substr(m_sPrefixPath.length(), sRequestPath.length() - m_sPrefixPath.length());
    // WsjcppLog::warn(_tag, sRequestPath2);
    if (sRequestPath2 == "") {
        sRequestPath2 = "index.html";
    }
    std::string sFilePath = m_sWebFolder + sRequestPath2;
    // WsjcppLog::warn(_tag, sFilePath);
    
    if (WsjcppCore::fileExists(sFilePath)) {
        WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
        resp.cacheSec(60).ok().sendFile(sFilePath);
    } else {
        WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
        resp.noCache().notFound().sendEmpty();
    }
    return true;
}