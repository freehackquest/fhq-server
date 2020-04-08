#include "wsjcpp_light_web_http_handler_rewrite_folder.h"
#include <wsjcpp_core.h>

// ----------------------------------------------------------------------

WsjcppLightWebHttpHandlerRewriteFolder::WsjcppLightWebHttpHandlerRewriteFolder(const std::string &sPrefixPath, const std::string &sWebFolder)
: WsjcppLightWebHttpHandlerBase("rewrite-folder") {

    TAG = "WsjcppLightWebHttpHandlerRewriteFolder";
    m_sPrefixPath = WsjcppCore::doNormalizePath(sPrefixPath + "/");
    m_sWebFolder = WsjcppCore::doNormalizePath(sWebFolder + "/");
}

// ----------------------------------------------------------------------

bool WsjcppLightWebHttpHandlerRewriteFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
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

bool WsjcppLightWebHttpHandlerRewriteFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WsjcppLog::warn(_tag, pRequest->requestPath());

    // cat subfolder
    std::string sRequestPath2 = sRequestPath.substr(m_sPrefixPath.length(), sRequestPath.length() - m_sPrefixPath.length());
    std::string sFilePath = m_sWebFolder + sRequestPath2;
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
