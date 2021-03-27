#include "wsjcpp_light_web_server.h"
#include <unistd.h>
#include <string.h>
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// WsjcppLightWebHttpHandlerBase

WsjcppLightWebHttpHandlerBase::WsjcppLightWebHttpHandlerBase(const std::string &sName) {
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &WsjcppLightWebHttpHandlerBase::name() {
    return m_sName;
}

// ----------------------------------------------------------------------
// WsjcppLightWebHttpThreadWorker

void* wsjcppLightWebServerProcessRequest(void *arg) {
    WsjcppLightWebHttpThreadWorker *pWorker = (WsjcppLightWebHttpThreadWorker *)arg;
    pthread_detach(pthread_self());
    pWorker->run();
    return 0;
}

// ----------------------------------------------------------------------

WsjcppLightWebHttpThreadWorker::WsjcppLightWebHttpThreadWorker(
    const std::string &sName, 
    WsjcppLightWebDequeHttpRequests *pDeque, 
    std::vector<WsjcppLightWebHttpHandlerBase *> *pVHandlers
) {
    TAG = "WsjcppLightWebHttpThreadWorker-" + sName;
    m_pDeque = pDeque;
    m_bStop = false;
    m_bStopped = true;
    m_sName = sName;
    m_pVHandlers = pVHandlers;
}

// ----------------------------------------------------------------------

void WsjcppLightWebHttpThreadWorker::start() {
    m_bStop = false;
    m_bStopped = false;
    WsjcppLog::info(TAG, "Start");
    pthread_create(&m_serverThread, NULL, &wsjcppLightWebServerProcessRequest, (void *)this);
}

// ----------------------------------------------------------------------

void WsjcppLightWebHttpThreadWorker::stop() {
    m_bStop = true;
}

// ----------------------------------------------------------------------

void WsjcppLightWebHttpThreadWorker::run() {
    const int nMaxPackageSize = 4096;
    while (1) {
        if (m_bStop) {
            m_bStopped = true;
            return;
        }
        WsjcppLightWebHttpRequest *pInfo = m_pDeque->popRequest();
        
        if (pInfo != nullptr) {
            int nSockFd = pInfo->getSockFd();
            WsjcppLog::info(TAG, "IP-address: " + pInfo->getAddress());

            // set timeout options
            struct timeval timeout;
            timeout.tv_sec = 1; // 1 seconds timeout
            timeout.tv_usec = 0;
            setsockopt(nSockFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            WsjcppLightWebHttpResponse *pResponse = new WsjcppLightWebHttpResponse(nSockFd);
            int n;
            // int newsockfd = (long)arg;
            char msg[nMaxPackageSize];

            std::string sRequest;
            
            // std::cout << nSockFd  << ": address = " << info->address() << "\n";
            // read data from socket
            bool bErrorRead = false;
            while (1) { // problem can be here
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
                WsjcppLog::info(TAG, "Readed " + std::to_string(n) + " bytes...");
                msg[n] = 0;
                sRequest = std::string(msg);

                std::string sRecv(msg);
                pInfo->appendRecieveRequest(sRecv);

                if (pInfo->isEnoughAppendReceived()) {
                    break;
                }
                // TODO redesign or switch to another socket
                // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            // TODO read and replace X-Forwarded-IP
            // TODO read and replace X-Forwarded-Host
            WsjcppLog::info(TAG, "\nRequest: \n>>>\n" + sRequest + "\n<<<");
            if (bErrorRead) {
                pResponse->requestTimeout().noCache().sendText(
                    "<html><body><h1>408 Request Time-out</h1>"
                    "Your browser didn't send a complete request in time."
                    "</body></html>"
                );
            } else if (pInfo->getRequestType() == "OPTIONS") {
                pResponse->ok().sendOptions("OPTIONS, GET, POST");
            } else if (pInfo->getRequestType() != "GET" && pInfo->getRequestType() != "POST") {
                pResponse->notImplemented().sendEmpty();
            } else {
                if (!this->handle(pInfo)) {
                    pResponse->notFound().sendEmpty();
                } else {
                    // TODO resp internal error
                    // this->response(WsjcppLightWebHttpResponse::RESP_INTERNAL_SERVER_ERROR);     
                }
            }
            delete pInfo;
            delete pResponse;
        }
        if (m_bStop) {
            m_bStopped = true;
            return;
        }
    }
    m_bStopped = true;
}

// ----------------------------------------------------------------------

bool WsjcppLightWebHttpThreadWorker::handle(WsjcppLightWebHttpRequest *pRequest) {
    std::vector<WsjcppLightWebHttpHandlerBase *>::iterator it; 
    for (it = m_pVHandlers->begin(); it < m_pVHandlers->end(); it++) {
        WsjcppLightWebHttpHandlerBase *pHandler = *it;
        if (pHandler->canHandle(m_sName, pRequest)) {
            if (pHandler->handle(m_sName, pRequest)) {
                return true;
            } else {
                WsjcppLog::warn(TAG, pHandler->name() + " - could not handle request '" + pRequest->getRequestPath() + "'");
            }
        }
    }
    return false;
}

// ----------------------------------------------------------------------
// WsjcppLightWebServer

WsjcppLightWebServer::WsjcppLightWebServer() {
    TAG = "WsjcppLightWebServer";
    m_nMaxWorkers = 4;
    m_pDeque = new WsjcppLightWebDequeHttpRequests();
    m_pVHandlers = new std::vector<WsjcppLightWebHttpHandlerBase *>();
    m_bStop = false;
    m_nPort = 8080;
}

// ----------------------------------------------------------------------

void WsjcppLightWebServer::setPort(int nPort) {
    // TODO use a port validators
    if (nPort > 10 && nPort < 65536) {
        m_nPort = nPort;
    } else {
        WsjcppLog::throw_err(TAG, "Port must be 10...65535");
    }
    m_nPort = nPort;
}

// ----------------------------------------------------------------------

void WsjcppLightWebServer::setMaxWorkers(int nMaxWorkers) {
    // TODO number validator
    if (nMaxWorkers > 0 && nMaxWorkers <= 100) {
        m_nMaxWorkers = nMaxWorkers;
    } else {
        WsjcppLog::warn(TAG, "Max workers must be 1...100");
    }
}

// ----------------------------------------------------------------------

void WsjcppLightWebServer::startSync() {
    
    m_nSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_nSockFd <= 0) {
        WsjcppLog::err(TAG, "Failed to establish socket connection");
        return;
    }
    int enable = 1;
    if (setsockopt(m_nSockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        WsjcppLog::err(TAG, "setsockopt(SO_REUSEADDR) failed");
        return;
    }

    memset(&m_serverAddress, 0, sizeof(m_serverAddress));
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    m_serverAddress.sin_port = htons(m_nPort);
    if (bind(m_nSockFd, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) == -1) {
        WsjcppLog::err(TAG, "Error binding to port " + std::to_string(m_nPort));
        return;
    }
    listen(m_nSockFd, 5);
    WsjcppLog::info("LightHttpServer", "Light Http Server started on " + std::to_string(m_nPort) + " port.");

    std::string str;
    m_bStop = false;
    this->checkAndRestartWorkers();
    while (!m_bStop) { // or problem can be here
        struct sockaddr_in clientAddress;
        socklen_t sosize  = sizeof(clientAddress);
        int nSockFd = accept(m_nSockFd,(struct sockaddr*)&clientAddress,&sosize);
        std::string sAddress = inet_ntoa(clientAddress.sin_addr);
        WsjcppLog::info(TAG, "Connected " + sAddress);
        WsjcppLightWebHttpRequest *pInfo = new WsjcppLightWebHttpRequest(nSockFd, sAddress);
        // info will be removed inside a thread
        m_pDeque->pushRequest(pInfo); // here will be unlocked workers
        // this->checkAndRestartWorkers();
        // TODO check workers if something stop - then restart
        // pthread_create(&m_serverThread, NULL, &newRequest, (void *)pInfo);
        // ???? accept must be lock this thread
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    m_pDeque->cleanup();
    this->stopAndRemoveWorkers();
    close(m_nSockFd);
}

// ----------------------------------------------------------------------

void* wsjcppLightWebServerProcessServerStart(void *arg) {
    WsjcppLightWebServer *pLightWebServer = (WsjcppLightWebServer *)arg;
    pthread_detach(pthread_self());
    pLightWebServer->startSync();
    return 0;
}

// ----------------------------------------------------------------------

void WsjcppLightWebServer::start() {
    m_bStop = false;
    pthread_create(&m_serverThread, NULL, &wsjcppLightWebServerProcessServerStart, (void *)this);
}

// ----------------------------------------------------------------------

void WsjcppLightWebServer::stop() {
    m_bStop = true;
} 

// ----------------------------------------------------------------------

void WsjcppLightWebServer::addHandler(WsjcppLightWebHttpHandlerBase *pHandler) {
    m_pVHandlers->push_back(pHandler);
}

// ----------------------------------------------------------------------

void WsjcppLightWebServer::checkAndRestartWorkers() {
    if (!m_bStop) {
        if (m_vWorkers.size() < m_nMaxWorkers) {
            int nSize = m_vWorkers.size();
            for (int i = nSize; i < m_nMaxWorkers; i++) {
                m_vWorkers.push_back(new WsjcppLightWebHttpThreadWorker("worker" + std::to_string(i), m_pDeque, m_pVHandlers));
            }
        }

        for (int i = 0; i < m_vWorkers.size(); i++) {
            WsjcppLightWebHttpThreadWorker *pWorker = m_vWorkers[i];
            pWorker->start();
        }
    }
}

// ----------------------------------------------------------------------

void WsjcppLightWebServer::stopAndRemoveWorkers() {
    if (m_bStop) {
        for (int i = 0; i < m_vWorkers.size(); i++) {
            m_vWorkers[i]->stop();
            delete m_vWorkers[i];
        }
        m_vWorkers.clear();
    }
}

// ----------------------------------------------------------------------

std::string WsjcppLightWebServer::readAddress(int nSockFd) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(nSockFd, (struct sockaddr *)&addr, &addr_size);
    char *clientip = new char[20];
    memset(clientip, 0, 20);
    strcpy(clientip, inet_ntoa(addr.sin_addr));
    return std::string(clientip);
}

// ----------------------------------------------------------------------