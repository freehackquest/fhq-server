#include "light_http_thread_worker.h"
#include <logger.h>
#include <light_http_handlers.h>
#include <light_http_response.h>
#include <thread>

// ----------------------------------------------------------------------

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
    
