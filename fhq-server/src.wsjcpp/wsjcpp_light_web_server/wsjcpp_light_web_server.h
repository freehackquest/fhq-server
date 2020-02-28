#ifndef WSJCPP_LIGHT_WEB_SERVER_H
#define WSJCPP_LIGHT_WEB_SERVER_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "wsjcpp_light_web_http_response.h"
#include "wsjcpp_light_web_http_request.h"
#include "wsjcpp_light_web_deque_http_requests.h"

// ---------------------------------------------------------------------

class WSJCppLightWebHttpHandlerBase {
    public:
        WSJCppLightWebHttpHandlerBase(const std::string &sName);
        const std::string &name();
        virtual bool canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) = 0;
        virtual bool handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) = 0;

    private:
        std::string m_sName;
};

// ---------------------------------------------------------------------

class WSJCppLightWebHttpThreadWorker {
    public:

        WSJCppLightWebHttpThreadWorker(
            const std::string &sName, 
            WSJCppLightWebDequeHttpRequests *pDeque, 
            std::vector<WSJCppLightWebHttpHandlerBase *> *pVHandlers
        );

        void start();
        void stop();
        void run();

    private:
        bool handle(WSJCppLightWebHttpRequest *pRequest);
        std::string TAG;
        std::string m_sName;
        WSJCppLightWebDequeHttpRequests *m_pDeque;
        std::vector<WSJCppLightWebHttpHandlerBase *> *m_pVHandlers;
        bool m_bStop;
        bool m_bStopped;
        pthread_t m_serverThread;
};

// ---------------------------------------------------------------------

class WSJCppLightWebServer {
    public:

        WSJCppLightWebServer();
        void setPort(int nPort);
        void setMaxWorkers(int nMaxWorkers);
        void startSync();
        void start();
        void stop();
        void addHandler(WSJCppLightWebHttpHandlerBase *pHandler);

    private:
        void checkAndRestartWorkers();
        void stopAndRemoveWorkers();
        std::string readAddress(int nSockFd);
        std::string TAG;
        WSJCppLightWebDequeHttpRequests *m_pDeque;
        bool m_bStop;

        int m_nMaxWorkers;
        int m_nPort;
        std::vector<WSJCppLightWebHttpHandlerBase *> *m_pVHandlers;
        std::vector<WSJCppLightWebHttpThreadWorker *> m_vWorkers;

        int m_nSockFd;
        struct sockaddr_in m_serverAddress;
        pthread_t m_serverThread;
};

#endif // WSJCPP_LIGHT_WEB_SERVER_H


