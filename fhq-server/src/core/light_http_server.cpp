#include "light_http_server.h"
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
#include <thread>
#include <algorithm>
#include <fallen.h>

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
	long nSec = Fallen::currentTime_seconds();
	m_sLastModified = Fallen::formatTimeForWeb(nSec);
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
// LightHttpRequest

LightHttpRequest::LightHttpRequest(int nSockFd, const std::string &sAddress) {
	m_nSockFd = nSockFd;
	m_sAddress = sAddress;
	m_bClosed = false;
	m_sRequest = "";
	m_nParserState = EnumParserState::START;
	TAG = "LightHttpRequest";
	long nSec = Fallen::currentTime_seconds();
	m_sLastModified = Fallen::formatTimeForWeb(nSec);
	m_nContentLength = 0;
}

// ----------------------------------------------------------------------

int LightHttpRequest::sockFd(){
	return m_nSockFd;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestType(){
	return m_sRequestType;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestPath(){
	return m_sRequestPath;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestHttpVersion(){
	return m_sRequestHttpVersion;
}

std::map<std::string,std::string> &LightHttpRequest::requestQueryParams() {
	return m_sRequestQueryParams;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::address(){
	return m_sAddress;
}

// ----------------------------------------------------------------------

void LightHttpRequest::appendRecieveRequest(const std::string &sRequestPart) {
	m_sRequest += sRequestPart;
	const std::string sContentLengthPrefix = "content-length:";
	if (m_nParserState == EnumParserState::START) {
		m_vHeaders.clear();
		// Log::info(TAG, "START \n>>>\n" + m_sRequest + "\n<<<\n");

		std::istringstream f(m_sRequest);
		std::string sLine = "";
		int nSize = 0;
		bool bHeadersEnded = false;
		while (getline(f, sLine, '\n')) {
			nSize += sLine.length() + 1;
			Fallen::trim(sLine);
			// Log::info(TAG, "Line: {" + sLine + "}, size=" + std::to_string(sLine.length()));
			if (sLine.length() == 0) {
				bHeadersEnded = true;
				break;
			}
			m_vHeaders.push_back(sLine);

			Fallen::to_lower(sLine);
			if (!sLine.compare(0, sContentLengthPrefix.size(), sContentLengthPrefix)) {
				m_nContentLength = atoi(sLine.substr(sContentLengthPrefix.size()).c_str());
				// Log::warn(TAG, "Content-Length: " + std::to_string(m_nContentLength));
			}
		}

		if (bHeadersEnded) {
			if (m_vHeaders.size() > 0) {
				this->parseFirstLine(m_vHeaders[0]);
			}
			m_sRequest.erase(0, nSize);
			// Log::info(TAG, "AFTER ERASE \n>>>\n" + m_sRequest + "\n<<<\n");
			m_nParserState = EnumParserState::BODY;
		} else {
			// Log::info(TAG, "Not ended");
		}
	}
	
	if (m_nParserState == EnumParserState::BODY && m_sRequest.length() == m_nContentLength) {
		m_nParserState = EnumParserState::ENDED;
		m_sRequestBody = m_sRequest;
	}
}

// ----------------------------------------------------------------------

bool LightHttpRequest::isEnoughAppendReceived() {
	return m_nParserState == EnumParserState::ENDED;
}

// ----------------------------------------------------------------------

void LightHttpRequest::parseFirstLine(const std::string &sHeader) {
	if (sHeader.size() > 0) {
		std::istringstream f(sHeader);
		std::vector<std::string> params;
		std::string s;
		while (getline(f, s, ' ')) {
			params.push_back(s);
		}
		if(params.size() > 0){
			m_sRequestType = params[0];
		}

		if(params.size() > 1){
			m_sRequestPath = params[1];
		}

		// TODO m_sRequestPath - need split by ? if exists
		if(params.size() > 2){
			m_sRequestHttpVersion = params[2];
		}
	}
	
	// parse query
	std::size_t nFound = m_sRequestPath.find("?");
  	if (nFound != std::string::npos) {
		std::string sQuery = m_sRequestPath.substr(nFound+1);
		m_sRequestPath = m_sRequestPath.substr(0, nFound);
		std::istringstream f(sQuery);
		std::string sParam;
		while (getline(f, sParam, '&')) {
			std::size_t nFound2 = sParam.find("=");
			std::string sValue = sParam.substr(nFound2+1);
			std::string sName = sParam.substr(0, nFound2);
			m_sRequestQueryParams[sName] = sValue; // wrong use map for params
		}
	}
}

// ----------------------------------------------------------------------
// LightHttpDequeRequests

LightHttpRequest *LightHttpDequeRequests::popRequest(){
	std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
	LightHttpRequest *pRequest = nullptr;
	int nSize = m_dequeRequests.size();
	if (nSize > 0) {
		pRequest = m_dequeRequests.back();
		m_dequeRequests.pop_back();
	}
	return pRequest;
}

// ----------------------------------------------------------------------

void LightHttpDequeRequests::pushRequest(LightHttpRequest *pRequest){
	std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
	if (m_dequeRequests.size() > 20) {
		Log::warn(TAG, " deque more than " + std::to_string(m_dequeRequests.size()));
	}
	m_dequeRequests.push_front(pRequest);
}

// ----------------------------------------------------------------------

void LightHttpDequeRequests::cleanup(){
	std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
	while (m_dequeRequests.size() > 0) {
		delete m_dequeRequests.back();
		m_dequeRequests.pop_back();
	}
}


// ---------------------------------------------------------------------
// LightHttpHandlerBase

LightHttpHandlerBase::LightHttpHandlerBase(const std::string &sName) {
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &LightHttpHandlerBase::name() {
    return m_sName;
}

// ----------------------------------------------------------------------
// LightHttpHandlers

// ---------------------------------------------------------------------

LightHttpHandlers::LightHttpHandlers() {
    TAG = "LightHttpHandlers";
}

// ---------------------------------------------------------------------

void LightHttpHandlers::add(LightHttpHandlerBase *pHandler) {
    m_pHandlers.push_back(pHandler);
}

// ---------------------------------------------------------------------

void LightHttpHandlers::remove(const std::string &sName) {
    std::vector<LightHttpHandlerBase *>::iterator it = m_pHandlers.begin();
    for (it = m_pHandlers.end(); it != m_pHandlers.begin(); it--) {
        LightHttpHandlerBase *pHandler = *it;
        if (pHandler->name() == sName) {
            m_pHandlers.erase(it);
        }
    }
}

// ---------------------------------------------------------------------

bool LightHttpHandlers::find(const std::string &sName, LightHttpHandlerBase *pHandler) {
    std::vector<LightHttpHandlerBase *>::iterator it = m_pHandlers.begin();
    for (it = m_pHandlers.begin(); it != m_pHandlers.end(); ++it) {
        LightHttpHandlerBase *p = *it;
        if (p->name() == sName) {
            pHandler = p;
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

bool LightHttpHandlers::handle(const std::string &sWorkerId, LightHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;

    for (int i = 0; i < m_pHandlers.size(); i++) {
        if (m_pHandlers[i]->canHandle(sWorkerId, pRequest)) {
            if (m_pHandlers[i]->handle(sWorkerId, pRequest)) {
                return true;
            } else {
                Log::warn("LightHttpHandlers", m_pHandlers[i]->name() + " - could not handle request '" + pRequest->requestPath() + "'");
            }
        }
    }
    return false;
}

// ----------------------------------------------------------------------
// LightHttpThreadWorker

void* processRequest(void *arg) {
	LightHttpThreadWorker *pWorker = (LightHttpThreadWorker *)arg;
	pthread_detach(pthread_self());
    pWorker->run();
	return 0;
}

// ----------------------------------------------------------------------

LightHttpThreadWorker::LightHttpThreadWorker(const std::string &sName, LightHttpDequeRequests *pDeque, LightHttpHandlers *pHandlers) {
    TAG = "LightHttpThreadWorker-" + sName;
    m_pDeque = pDeque;
    m_bStop = false;
    m_sName = sName;
    m_pHandlers = pHandlers;
}

// ----------------------------------------------------------------------

void LightHttpThreadWorker::start() {
    m_bStop = false;
    pthread_create(&m_serverThread, NULL, &processRequest, (void *)this);
}

// ----------------------------------------------------------------------

void LightHttpThreadWorker::stop() {
    m_bStop = true;
}

// ----------------------------------------------------------------------

void LightHttpThreadWorker::run() {
    const int nMaxPackageSize = 4096;
    while(1) {
		if (m_bStop) return;
        LightHttpRequest *pInfo = m_pDeque->popRequest();
		bool bExists = pInfo != nullptr;
        // TODO refactor
		if (bExists) {
            int nSockFd = pInfo->sockFd();

            // set timeout options
            struct timeval timeout;
            timeout.tv_sec = 1; // 1 seconds timeout
            timeout.tv_usec = 0;
            setsockopt(nSockFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            struct sockaddr_in addr;
            socklen_t addr_size = sizeof(struct sockaddr_in);
            int res = getpeername(nSockFd, (struct sockaddr *)&addr, &addr_size);
            char *clientip = new char[20];
            memset(clientip, 0, 20);
            strcpy(clientip, inet_ntoa(addr.sin_addr));
            Log::info(TAG, "IP-address: " + std::string(clientip));

            LightHttpResponse *pResponse = new LightHttpResponse(nSockFd);
            int n;
            // int newsockfd = (long)arg;
            char msg[nMaxPackageSize];

            std::string sRequest;
            
            // std::cout << nSockFd  << ": address = " << info->address() << "\n";
            // read data from socket
            bool bErrorRead = false;
            while(1) { // problem can be here
                // std::cout << nSockFd  << ": wait recv...\n";
                memset(msg, 0, nMaxPackageSize);

                n = recv(nSockFd, msg, nMaxPackageSize, 0);
                // std::cout << "N: " << n << std::endl;
                if (n == -1) {
                    bErrorRead = true;
                    std::cout << nSockFd  << ": error read... \n";
                    break;
                }
                if (n == 0) {
                    //close(nSockFd);
                    break;
                }
                Log::info(TAG, "Readed " + std::to_string(n) + " bytes...");
                msg[n] = 0;
                //send(newsockfd,msg,n,0);
                sRequest = std::string(msg);

                std::string sRecv(msg);
                pInfo->appendRecieveRequest(sRecv);

                if (pInfo->isEnoughAppendReceived()) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            Log::info(TAG, "\nRequest: \n>>>\n" + sRequest + "\n<<<");

            if (bErrorRead) {
                pResponse->sendDontUnderstand();
            } else if (pInfo->requestType() == "OPTIONS") {
                pResponse->ok().sendOptions("OPTIONS, GET, POST");
            } else if (pInfo->requestType() != "GET" && pInfo->requestType() != "POST") {
                pResponse->notImplemented().sendEmpty();
            } else {
                if (!m_pHandlers->handle(m_sName, pInfo)) {
                    pResponse->notFound().sendEmpty();
                } else {
                    // TODO resp internal error
                    // this->response(LightHttpResponse::RESP_INTERNAL_SERVER_ERROR);     
                }
            }
			delete pInfo;
            delete pResponse;
		}

		if (!bExists) {
            if (m_bStop) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (m_bStop) return;
		}
	}
}


// ----------------------------------------------------------------------
// LightHttpServer

LightHttpServer::LightHttpServer() {
	TAG = "LightHttpServer";
	m_nMaxWorkers = 4;
	m_pDeque = new LightHttpDequeRequests();
	m_pHandlers = new LightHttpHandlers();
	m_bStop = false;
	m_nPort = 8080;
}

// ----------------------------------------------------------------------

void LightHttpServer::setPort(int nPort) {
	if (nPort > 10 && nPort < 65536) {
		m_nPort = nPort;
	} else {
		Log::warn(TAG, "Port must be 10...65535");
	}

	m_nPort = nPort;
}

// ----------------------------------------------------------------------

void LightHttpServer::setMaxWorkers(int nMaxWorkers) {
	if (nMaxWorkers > 0 && nMaxWorkers <= 100) {
		m_nMaxWorkers = nMaxWorkers;
	} else {
		Log::warn(TAG, "Max workers must be 1...100");
	}
}

// ----------------------------------------------------------------------

void LightHttpServer::startSync() {
	
	m_nSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(m_nSockFd <= 0){
		Log::err(TAG, "Failed to establish socket connection");
		return;
	}
	int enable = 1;
	if (setsockopt(m_nSockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		Log::err(TAG, "setsockopt(SO_REUSEADDR) failed");
		return;
	}

 	memset(&m_serverAddress, 0, sizeof(m_serverAddress));
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serverAddress.sin_port = htons(m_nPort);
	if(bind(m_nSockFd, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) == -1){
		Log::err(TAG, "Error binding to port " + std::to_string(m_nPort));
		return;
	}
 	listen(m_nSockFd, 5);
	Log::info("LightHttpServer", "Light Http Server started on " + std::to_string(m_nPort) + " port.");

	for (int i = 0; i < m_nMaxWorkers; i++) {
		m_vWorkers.push_back(new LightHttpThreadWorker("worker" + std::to_string(i), m_pDeque, m_pHandlers));
	}

	for (int i = 0; i < m_vWorkers.size(); i++) {
		m_vWorkers[i]->start();
	}

	std::string str;
	m_bStop = false;
	while(!m_bStop) { // or problem can be here
		struct sockaddr_in clientAddress;
		socklen_t sosize  = sizeof(clientAddress);
		int nSockFd = accept(m_nSockFd,(struct sockaddr*)&clientAddress,&sosize);
		std::string sAddress = inet_ntoa(clientAddress.sin_addr);
		LightHttpRequest *pInfo = new LightHttpRequest(nSockFd, sAddress);
		// info will be removed inside a thread
		m_pDeque->pushRequest(pInfo);

		// pthread_create(&m_serverThread, NULL, &newRequest, (void *)pInfo);
		// std::cout << "wait \n";
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	m_pDeque->cleanup();
	for (int i = 0; i < m_vWorkers.size(); i++) {
		m_vWorkers[i]->stop();
	}
	close(m_nSockFd);
}

// ----------------------------------------------------------------------

void* processWebServerStart(void *arg) {
	LightHttpServer *pWebServer = (LightHttpServer *)arg;
	pthread_detach(pthread_self());
    pWebServer->startSync();
	return 0;
}

// ----------------------------------------------------------------------

void LightHttpServer::start() {
	m_bStop = false;
    pthread_create(&m_serverThread, NULL, &processWebServerStart, (void *)this);
}
// ----------------------------------------------------------------------

LightHttpHandlers *LightHttpServer::handlers() {
	return m_pHandlers;
}

// ----------------------------------------------------------------------

void LightHttpServer::stop() {
	m_bStop = true;
} 
