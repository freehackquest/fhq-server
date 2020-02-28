#include "wsjcpp_light_web_server.h"
#include <unistd.h>
#include <string.h>
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// WSJCppLightWebHttpHandlerBase

WSJCppLightWebHttpHandlerBase::WSJCppLightWebHttpHandlerBase(const std::string &sName) {
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &WSJCppLightWebHttpHandlerBase::name() {
    return m_sName;
}

// ----------------------------------------------------------------------
// WSJCppLightWebHttpThreadWorker

void* processRequest(void *arg) {
    WSJCppLightWebHttpThreadWorker *pWorker = (WSJCppLightWebHttpThreadWorker *)arg;
    pthread_detach(pthread_self());
    pWorker->run();
    return 0;
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpThreadWorker::WSJCppLightWebHttpThreadWorker(
    const std::string &sName, 
    WSJCppLightWebDequeHttpRequests *pDeque, 
    std::vector<WSJCppLightWebHttpHandlerBase *> *pVHandlers
) {
    TAG = "WSJCppLightWebHttpThreadWorker-" + sName;
    m_pDeque = pDeque;
    m_bStop = false;
    m_bStopped = true;
    m_sName = sName;
    m_pVHandlers = pVHandlers;
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpThreadWorker::start() {
    m_bStop = false;
    m_bStopped = false;
    WSJCppLog::info(TAG, "Start");
    pthread_create(&m_serverThread, NULL, &processRequest, (void *)this);
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpThreadWorker::stop() {
    m_bStop = true;
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpThreadWorker::run() {
    const int nMaxPackageSize = 4096;
    while (1) {
        if (m_bStop) {
            m_bStopped = true;
            return;
        }
        WSJCppLightWebHttpRequest *pInfo = m_pDeque->popRequest();
        
        if (pInfo != nullptr) {
            int nSockFd = pInfo->getSockFd();
            WSJCppLog::info(TAG, "IP-address: " + pInfo->getAddress());

            // set timeout options
            struct timeval timeout;
            timeout.tv_sec = 1; // 1 seconds timeout
            timeout.tv_usec = 0;
            setsockopt(nSockFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            WSJCppLightWebHttpResponse *pResponse = new WSJCppLightWebHttpResponse(nSockFd);
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
                WSJCppLog::info(TAG, "Readed " + std::to_string(n) + " bytes...");
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
            WSJCppLog::info(TAG, "\nRequest: \n>>>\n" + sRequest + "\n<<<");
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
                    // this->response(WSJCppLightWebHttpResponse::RESP_INTERNAL_SERVER_ERROR);     
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

bool WSJCppLightWebHttpThreadWorker::handle(WSJCppLightWebHttpRequest *pRequest) {
    std::vector<WSJCppLightWebHttpHandlerBase *>::iterator it; 
    for (it = m_pVHandlers->begin(); it < m_pVHandlers->end(); it++) {
        WSJCppLightWebHttpHandlerBase *pHandler = *it;
        if (pHandler->canHandle(m_sName, pRequest)) {
            if (pHandler->handle(m_sName, pRequest)) {
                return true;
            } else {
                WSJCppLog::warn(TAG, pHandler->name() + " - could not handle request '" + pRequest->getRequestPath() + "'");
            }
        }
    }
    return false;
}

// ----------------------------------------------------------------------
// WSJCppLightWebServer

WSJCppLightWebServer::WSJCppLightWebServer() {
    TAG = "WSJCppLightWebServer";
    m_nMaxWorkers = 4;
    m_pDeque = new WSJCppLightWebDequeHttpRequests();
    m_pVHandlers = new std::vector<WSJCppLightWebHttpHandlerBase *>();
    m_bStop = false;
    m_nPort = 8080;
}

// ----------------------------------------------------------------------

void WSJCppLightWebServer::setPort(int nPort) {
    // TODO use a port validators
    if (nPort > 10 && nPort < 65536) {
        m_nPort = nPort;
    } else {
        WSJCppLog::throw_err(TAG, "Port must be 10...65535");
    }
    m_nPort = nPort;
}

// ----------------------------------------------------------------------

void WSJCppLightWebServer::setMaxWorkers(int nMaxWorkers) {
    // TODO number validator
    if (nMaxWorkers > 0 && nMaxWorkers <= 100) {
        m_nMaxWorkers = nMaxWorkers;
    } else {
        WSJCppLog::warn(TAG, "Max workers must be 1...100");
    }
}

// ----------------------------------------------------------------------

void WSJCppLightWebServer::startSync() {
    
    m_nSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_nSockFd <= 0) {
        WSJCppLog::err(TAG, "Failed to establish socket connection");
        return;
    }
    int enable = 1;
    if (setsockopt(m_nSockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        WSJCppLog::err(TAG, "setsockopt(SO_REUSEADDR) failed");
        return;
    }

    memset(&m_serverAddress, 0, sizeof(m_serverAddress));
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    m_serverAddress.sin_port = htons(m_nPort);
    if (bind(m_nSockFd, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) == -1) {
        WSJCppLog::err(TAG, "Error binding to port " + std::to_string(m_nPort));
        return;
    }
    listen(m_nSockFd, 5);
    WSJCppLog::info("LightHttpServer", "Light Http Server started on " + std::to_string(m_nPort) + " port.");

    std::string str;
    m_bStop = false;
    this->checkAndRestartWorkers();
    while (!m_bStop) { // or problem can be here
        struct sockaddr_in clientAddress;
        socklen_t sosize  = sizeof(clientAddress);
        int nSockFd = accept(m_nSockFd,(struct sockaddr*)&clientAddress,&sosize);
        std::string sAddress = inet_ntoa(clientAddress.sin_addr);
        WSJCppLog::info(TAG, "Connected " + sAddress);
        WSJCppLightWebHttpRequest *pInfo = new WSJCppLightWebHttpRequest(nSockFd, sAddress);
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

void* processWebServerStart(void *arg) {
    WSJCppLightWebServer *pLightWebServer = (WSJCppLightWebServer *)arg;
    pthread_detach(pthread_self());
    pLightWebServer->startSync();
    return 0;
}

// ----------------------------------------------------------------------

void WSJCppLightWebServer::start() {
    m_bStop = false;
    pthread_create(&m_serverThread, NULL, &processWebServerStart, (void *)this);
}

// ----------------------------------------------------------------------

void WSJCppLightWebServer::stop() {
    m_bStop = true;
} 

// ----------------------------------------------------------------------

void WSJCppLightWebServer::addHandler(WSJCppLightWebHttpHandlerBase *pHandler) {
    m_pVHandlers->push_back(pHandler);
}

// ----------------------------------------------------------------------

void WSJCppLightWebServer::checkAndRestartWorkers() {
    if (!m_bStop) {
        if (m_vWorkers.size() < m_nMaxWorkers) {
            int nSize = m_vWorkers.size();
            for (int i = nSize; i < m_nMaxWorkers; i++) {
                m_vWorkers.push_back(new WSJCppLightWebHttpThreadWorker("worker" + std::to_string(i), m_pDeque, m_pVHandlers));
            }
        }

        for (int i = 0; i < m_vWorkers.size(); i++) {
            WSJCppLightWebHttpThreadWorker *pWorker = m_vWorkers[i];
            pWorker->start();
        }
    }
}

// ----------------------------------------------------------------------

void WSJCppLightWebServer::stopAndRemoveWorkers() {
    if (m_bStop) {
        for (int i = 0; i < m_vWorkers.size(); i++) {
            m_vWorkers[i]->stop();
            delete m_vWorkers[i];
        }
        m_vWorkers.clear();
    }
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebServer::readAddress(int nSockFd) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(nSockFd, (struct sockaddr *)&addr, &addr_size);
    char *clientip = new char[20];
    memset(clientip, 0, 20);
    strcpy(clientip, inet_ntoa(addr.sin_addr));
    return std::string(clientip);
}

// ----------------------------------------------------------------------