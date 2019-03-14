#include "light_http_response.h"
#include <logger.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>
#include <fstream>
#include <regex>        // regex, sregex_token_iterator
#include <stdio.h>
#include <math.h>
#include <fs.h>
#include <ts.h>

// ----------------------------------------------------------------------
// LightHttpResponse

// enum for http responses
std::map<int, std::string> *LightHttpResponse::g_mapReponseDescription = nullptr;

// deprecated
std::string LightHttpResponse::RESP_OK = "HTTP/1.1 200 OK";
std::string LightHttpResponse::RESP_BAD_REQUEST = "HTTP/1.1 400 Bad Request";
std::string LightHttpResponse::RESP_FORBIDDEN = "HTTP/1.1 403 Forbidden";
std::string LightHttpResponse::RESP_NOT_FOUND = "HTTP/1.1 404 Not Found";
std::string LightHttpResponse::RESP_PAYLOAD_TOO_LARGE = "HTTP/1.1 413 Payload Too Large";
std::string LightHttpResponse::RESP_INTERNAL_SERVER_ERROR = "HTTP/1.1 500 Internal Server Error";
std::string LightHttpResponse::RESP_NOT_IMPLEMENTED = "HTTP/1.1 501 Not Implemented";

// ----------------------------------------------------------------------

LightHttpResponse::LightHttpResponse(int nSockFd) {
    TAG = "LightHttpResponse";
    if (LightHttpResponse::g_mapReponseDescription == nullptr) {
        LightHttpResponse::g_mapReponseDescription = new std::map<int, std::string>();
        LightHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(200,"HTTP/1.1 200 OK"));
        LightHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(400, "HTTP/1.1 400 Bad Request"));
        LightHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(403, "HTTP/1.1 403 Forbidden"));
        LightHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(404, "HTTP/1.1 404 Not Found"));
        LightHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(413, "HTTP/1.1 413 Payload Too Large"));
        LightHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(500, "HTTP/1.1 500 Internal Server Error"));
        LightHttpResponse::g_mapReponseDescription->insert(std::pair<int, std::string>(501, "HTTP/1.1 501 Not Implemented"));
    }

    m_nSockFd = nSockFd;
	m_bClosed = false;
	noCache();
	long nSec = TS::currentTime_seconds();
	m_sLastModified = TS::formatTimeForWeb(nSec);
    m_nResponseCode = 500;
    m_sDataType = "text/html";
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::ok() {
    m_nResponseCode = 200;
    return *this;
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::badRequest() {
    m_nResponseCode = 400;
    return *this;
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::forbidden() {
    m_nResponseCode = 403;
    return *this;
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::notFound() {
    m_nResponseCode = 404;
    return *this;
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::payloadTooLarge() {
    m_nResponseCode = 413;
    return *this;
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::internalServerError() {
    m_nResponseCode = 500;
    return *this;
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::notImplemented() {
    m_nResponseCode = 501;
    return *this;
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::noCache() {
    m_sCacheControl = "no-cache, no-store, must-revalidate";
    return *this;
}

// ----------------------------------------------------------------------

LightHttpResponse &LightHttpResponse::cacheSec(int nCacheSec) {
    m_sCacheControl = "max-age=" + std::to_string(nCacheSec);
    return *this;
}

// ----------------------------------------------------------------------

std::string LightHttpResponse::prepareHeaders(int nLength) {
    std::string sResponseCode = LightHttpResponse::g_mapReponseDescription->at(m_nResponseCode);
    return sResponseCode + "\r\n"
		"Date: " + m_sLastModified + "\r\n"
		"Server: mecd\r\n"
        "Access-Control-Allow-Origin: *\r\n"
		"Cache-Control: " + m_sCacheControl + "\r\n"
		"Last-Modified: " + m_sLastModified + "\r\n" // TODO generate data
		"Content-Length: " + std::to_string(nLength) + "\r\n"
		"Content-Type: " + m_sDataType + "\r\n"
		"Connection: Closed\r\n";
}

// ----------------------------------------------------------------------

std::string LightHttpResponse::detectTypeOfFile(const std::string &sFilePath) {
    // TODO cache: check file in cache 
	std::string sFileExt = sFilePath.substr(sFilePath.find_last_of(".") + 1);

	std::string sType = "application/octet-stream";
	if (sFileExt == "json"){
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

void LightHttpResponse::sendText(const std::string &sBody) {
    m_sDataType = "text/html";
    
	std::string sResponse = prepareHeaders(sBody.length())
		+ "\r\n" + sBody;
	
	if(m_bClosed) {
		Log::warn(TAG, "Already sended response");
		return;
	}
	m_bClosed = true;
	
	Log::info(TAG, "\nResponse: \n>>>\n" + sResponse + "\n<<<");

	send(m_nSockFd, sResponse.c_str(), sResponse.length(),0);
	close(m_nSockFd);
}

// ----------------------------------------------------------------------

void LightHttpResponse::sendEmpty() {
    this->sendText("");
}

// ----------------------------------------------------------------------

void LightHttpResponse::sendOptions(const std::string &sOptions) {
    m_sDataType = "text/html";
	std::string sResponse = prepareHeaders(0)
        + "Access-Control-Allow-Methods: " + sOptions
		+ "\r\n\r\n";
	
	if(m_bClosed) {
		Log::warn(TAG, "Already sended response");
		return;
	}
	m_bClosed = true;
	
	Log::info(TAG, "\nResponse: \n>>>\n" + sResponse + "\n<<<");

	send(m_nSockFd, sResponse.c_str(), sResponse.length(),0);
	close(m_nSockFd);
}

// ----------------------------------------------------------------------

void LightHttpResponse::sendDontUnderstand() {
    std::string sResponse = "I don't understand you! Are you just a machine? Or maybe hacker?";
	
	if(m_bClosed) {
		Log::warn(TAG, "Already sended response");
		return;
	}
	m_bClosed = true;
	
	Log::info(TAG, "\nResponse: \n>>>\n" + sResponse + "\n<<<");

	send(m_nSockFd, sResponse.c_str(), sResponse.length(),0);
	close(m_nSockFd);
}

// ----------------------------------------------------------------------

void LightHttpResponse::sendFile(const std::string &sFilePath) {

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

void LightHttpResponse::sendBuffer(const std::string &sFilePath, const char *pBuffer, const int nBufferSize) {
    	
	// TODO cache: check file in cache 
	m_sDataType = detectTypeOfFile(sFilePath);
    
	std::string sResponse = prepareHeaders(nBufferSize)
		+ "\r\n";

	if(m_bClosed) {
		Log::warn(TAG, "Already sended response");
		// delete[] pData;
		return;
	}
	m_bClosed = true;
	#if __APPLE__
		send(m_nSockFd, sResponse.c_str(), sResponse.length(),SO_NOSIGPIPE);
		send(m_nSockFd, pData, nSize, SO_NOSIGPIPE);
	// #if
	// TARGET_OS_MAC 

	#else
		send(m_nSockFd, sResponse.c_str(), sResponse.length(), MSG_CONFIRM);
		send(m_nSockFd, pBuffer, nBufferSize, MSG_CONFIRM);
	#endif
	
	close(m_nSockFd);
}

// ----------------------------------------------------------------------
