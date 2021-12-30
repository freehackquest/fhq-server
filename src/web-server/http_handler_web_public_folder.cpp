#include "http_handler_web_public_folder.h"

#include <wsjcpp_core.h>
#include <employ_files.h>

// ----------------------------------------------------------------------

HttpHandlerWebPublicFolder::HttpHandlerWebPublicFolder(const std::string &sWebPublicFolder)
    : WsjcppLightWebHttpHandlerBase("web-public-folder") {

    TAG = "HttpHandlerWebPublicFolder";
    m_sWebPublicFolder = sWebPublicFolder;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebPublicFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->getRequestPath();

    if (!WsjcppCore::dirExists(m_sWebPublicFolder)) {
        WsjcppLog::warn(_tag, "Directory '" + m_sWebPublicFolder + "' does not exists");
    }
    if (sRequestPath.rfind("/public/", 0) == 0) {
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebPublicFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());

    std::string sRequestPath = pRequest->getRequestPath();
    sRequestPath = sRequestPath.substr(8); // remove '/public/'
    sRequestPath = WsjcppCore::doNormalizePath("/" + sRequestPath);

    // TODO redsign this hardcode
    std::string sFilename = "";
    if (sRequestPath.rfind("/public/", 0) == 0) {
        auto pEmployFiles = findWsjcppEmploy<EmployFiles>();
        ModelQuestFile model;
        if (!pEmployFiles->findQuestFileByFilePath(sRequestPath, model)) {
            std::string sMessageError = "This file not registered in the system '" + sRequestPath + "'";
            WsjcppLog::err(TAG, sMessageError);
            resp.cacheSec(0).notFound().sendText("<h1>" + sMessageError + "</h1>");
            return true;
        }
        sFilename = model.getFileName();
    }

    std::string sFilePath = m_sWebPublicFolder + sRequestPath; // TODO check /../ in path
    if (!WsjcppCore::fileExists(sFilePath)) {
        std::string sMessageError = "File not found '" + sRequestPath + "'";
        WsjcppLog::err(TAG, sMessageError);
        resp.cacheSec(0).notFound().sendText("<h1>" + sMessageError + "</h1>");
        return true;
    }
    resp.cacheSec(0).ok().sendFile(sFilePath, sFilename);
    return true;
}
