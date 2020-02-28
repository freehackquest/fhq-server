#include "http_handler_web_public_folder.h"
#include <fallen.h>

// ----------------------------------------------------------------------

HttpHandlerWebPublicFolder::HttpHandlerWebPublicFolder(const std::string &sWebPublicFolder)
    : WSJCppLightWebHttpHandlerBase("web-public-folder") {

    TAG = "HttpHandlerWebPublicFolder";
    m_sWebPublicFolder = sWebPublicFolder;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebPublicFolder::canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // WSJCppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->getRequestPath();

    if (!WSJCppCore::dirExists(m_sWebPublicFolder)) {
        WSJCppLog::warn(_tag, "Directory '" + m_sWebPublicFolder + "' does not exists");
    }
    if (sRequestPath.rfind("/public/", 0) != 0) {
        return false;
    }

    sRequestPath = sRequestPath.substr(7); // remove /public
    std::string sFilePath = m_sWebPublicFolder + sRequestPath; // TODO check /../ in path
    // WSJCppLog::warn(_tag, "Response Resources " + sFilePath);
    if (!WSJCppCore::fileExists(sFilePath)) { // TODO check the file exists not dir
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebPublicFolder::handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    sRequestPath = sRequestPath.substr(7); // remove /public

    std::string sFilePath = m_sWebPublicFolder + sRequestPath; // TODO check /../ in path
    if (!WSJCppCore::fileExists(sFilePath)) {
        WSJCppLightWebHttpResponse resp(pRequest->getSockFd());
        resp.cacheSec(0).notFound();
        return true;
    }
    WSJCppLightWebHttpResponse resp(pRequest->getSockFd());
    resp.cacheSec(0).ok().sendFile(sFilePath);
    return true;
}
