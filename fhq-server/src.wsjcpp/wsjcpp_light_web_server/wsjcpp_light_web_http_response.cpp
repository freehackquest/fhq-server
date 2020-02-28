#include "wsjcpp_light_web_http_response.h"
#include <wsjcpp_core.h>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/socket.h>

// ----------------------------------------------------------------------
// WSJCppLightWebHttpResponse

// enum for http responses
std::map<int, std::string> *WSJCppLightWebHttpResponse::g_mapReponseDescription = nullptr;

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse::WSJCppLightWebHttpResponse(int nSockFd) {
    TAG = "WSJCppLightWebHttpResponse";
    if (WSJCppLightWebHttpResponse::g_mapReponseDescription == nullptr) {
        WSJCppLightWebHttpResponse::g_mapReponseDescription = new std::map<int, std::string>();
        WSJCppLightWebHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(200,"HTTP/1.1 200 OK"));
        WSJCppLightWebHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(400, "HTTP/1.1 400 Bad Request"));
        WSJCppLightWebHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(403, "HTTP/1.1 403 Forbidden"));
        WSJCppLightWebHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(404, "HTTP/1.1 404 Not Found"));
        WSJCppLightWebHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(413, "HTTP/1.1 413 Payload Too Large"));
        WSJCppLightWebHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(500, "HTTP/1.1 500 Internal Server Error"));
        WSJCppLightWebHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(501, "HTTP/1.1 501 Not Implemented"));
        WSJCppLightWebHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(408, "HTTP/1.1 408 Request Time-out"));
    }

    m_nSockFd = nSockFd;
    m_bClosed = false;
    noCache();
    long nSec = WSJCppCore::currentTime_seconds();
    m_sLastModified = WSJCppCore::formatTimeForWeb(nSec);
    m_nResponseCode = 500;
    m_sDataType = "text/html";
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::ok() {
    m_nResponseCode = 200;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::badRequest() {
    m_nResponseCode = 400;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::forbidden() {
    m_nResponseCode = 403;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::notFound() {
    m_nResponseCode = 404;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::payloadTooLarge() {
    m_nResponseCode = 413;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::internalServerError() {
    m_nResponseCode = 500;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::notImplemented() {
    m_nResponseCode = 501;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::requestTimeout() {
    m_nResponseCode = 408;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::noCache() {
    m_sCacheControl = "no-cache, no-store, must-revalidate";
    return *this;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpResponse &WSJCppLightWebHttpResponse::cacheSec(int nCacheSec) {
    m_sCacheControl = "max-age=" + std::to_string(nCacheSec);
    return *this;
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpResponse::prepareHeaders(int nLength) {
    std::string sResponseCode = WSJCppLightWebHttpResponse::g_mapReponseDescription->at(m_nResponseCode);
    return sResponseCode + "\r\n"
        "Date: " + m_sLastModified + "\r\n"
        "Server: wsjcpp\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Cache-Control: " + m_sCacheControl + "\r\n"
        "Last-Modified: " + m_sLastModified + "\r\n" // TODO generate data
        "Content-Length: " + std::to_string(nLength) + "\r\n"
        "Content-Type: " + m_sDataType + "\r\n"
        "Connection: Closed\r\n";
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpResponse::detectTypeOfFile(const std::string &sFilePath) {
    // TODO cache: check file in cache 
    std::string sFileExt = sFilePath.substr(sFilePath.find_last_of(".") + 1);

    std::string sType = "application/octet-stream";
    if (sFileExt == "json") {
        sType = "application/json";
    } else if (sFileExt == "css") {
        sType = "text/css"; 
    } else if (sFileExt == "js") {
        sType = "text/javascript";
    } else if (sFileExt == "html") {
        sType = "text/html";
    } else if (sFileExt == "gif") {
        sType = "image/gif";
    } else if (sFileExt == "ico") {
        sType = "image/x-icon";
    } else if (sFileExt == "xml") {
        sType = "application/xml";
    } else if (sFileExt == "png") {
        sType = "application/xml";
    } else if (sFileExt == "jpg" || sFileExt == "jpeg") {
        sType = "image/jpeg";
    } else if (sFileExt == "svg") {
        sType = "image/svg+xml";
    }
    return sType;
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpResponse::sendText(const std::string &sBody) {
    m_sDataType = "text/html";
    
    std::string sResponse = prepareHeaders(sBody.length())
        + "\r\n" + sBody;
    
    if (m_bClosed) {
        WSJCppLog::warn(TAG, "Already sended response");
        return;
    }
    m_bClosed = true;
    
    WSJCppLog::info(TAG, "\nResponse: \n>>>\n" + sResponse + "\n<<<");

    send(m_nSockFd, sResponse.c_str(), sResponse.length(),0);
    close(m_nSockFd);
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpResponse::sendJson(const nlohmann::json &json) {
    m_sDataType = "application/json";
    std::string sBody = json.dump();
    std::string sResponse = prepareHeaders(sBody.length())
        + "\r\n" + sBody;
    
    if (m_bClosed) {
        WSJCppLog::warn(TAG, "Already sended response");
        return;
    }
    m_bClosed = true;
    
    WSJCppLog::info(TAG, "\nResponse: \n>>>\n" + sResponse + "\n<<<");

    send(m_nSockFd, sResponse.c_str(), sResponse.length(),0);
    close(m_nSockFd);
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpResponse::sendEmpty() {
    this->sendText("");
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpResponse::sendOptions(const std::string &sOptions) {
    m_sDataType = "text/html";
    std::string sResponse = prepareHeaders(0)
        + "Access-Control-Allow-Methods: " + sOptions
        + "\r\n\r\n";
    
    if (m_bClosed) {
        WSJCppLog::warn(TAG, "Already sended response");
        return;
    }
    m_bClosed = true;
    
    WSJCppLog::info(TAG, "\nResponse: \n>>>\n" + sResponse + "\n<<<");

    send(m_nSockFd, sResponse.c_str(), sResponse.length(),0);
    close(m_nSockFd);
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpResponse::sendFile(const std::string &sFilePath) {

    // read data from file
    std::ifstream f(sFilePath, std::ios::binary | std::ios::ate);
    std::streamsize nSize = f.tellg();
    f.seekg(0, std::ios::beg);
    char *pData = new char[nSize];
    // std::vector<char> buffer(size);
    if (nSize > 10*1024*1024) {
        this->payloadTooLarge();
        this->sendEmpty();
        delete[] pData;
        return;
    }

    if (!f.read(pData, nSize)) {
        this->forbidden();
        this->sendEmpty();
        delete[] pData;
        return;
        // std::cout << sFilePath << "\n filesize: " << nSize << " bytes\n";
    }

    this->sendBuffer(sFilePath, pData, nSize);
    delete[] pData;
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpResponse::sendBuffer(const std::string &sFilePath, const char *pBuffer, const int nBufferSize) {
        
    // TODO cache: check file in cache 
    m_sDataType = detectTypeOfFile(sFilePath);
    
    std::string sResponse = prepareHeaders(nBufferSize)
        + "\r\n";

    if (m_bClosed) {
        WSJCppLog::warn(TAG, "Already sended response");
        // delete[] pData;
        return;
    }
    m_bClosed = true;
    write(m_nSockFd, sResponse.c_str(), sResponse.length());
    write(m_nSockFd, pBuffer, nBufferSize);
    close(m_nSockFd);
}
