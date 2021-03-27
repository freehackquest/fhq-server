#ifndef WSJCPP_LIGHT_WEB_SERVER_H
#define WSJCPP_LIGHT_WEB_SERVER_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "wsjcpp_light_web_http_response.h"
#include "wsjcpp_light_web_http_request.h"
#include "wsjcpp_light_web_deque_http_requests.h"

// ---------------------------------------------------------------------

class WsjcppLightWebHttpHandlerBase {
    public:
        WsjcppLightWebHttpHandlerBase(const std::string &sName);
        const std::string &name();
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) = 0;
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) = 0;

    private:
        std::string m_sName;
};

// ---------------------------------------------------------------------

class WsjcppLightWebHttpThreadWorker {
    public:

        WsjcppLightWebHttpThreadWorker(
            const std::string &sName, 
            WsjcppLightWebDequeHttpRequests *pDeque, 
            std::vector<WsjcppLightWebHttpHandlerBase *> *pVHandlers
        );

        void start();
        void stop();
        void run();

    private:
        bool handle(WsjcppLightWebHttpRequest *pRequest);
        std::string TAG;
        std::string m_sName;
        WsjcppLightWebDequeHttpRequests *m_pDeque;
        std::vector<WsjcppLightWebHttpHandlerBase *> *m_pVHandlers;
        bool m_bStop;
        bool m_bStopped;
        pthread_t m_serverThread;
};

// ---------------------------------------------------------------------

class WsjcppLightWebServer {
    public:

        WsjcppLightWebServer();
        void setPort(int nPort);
        void setMaxWorkers(int nMaxWorkers);
        void startSync();
        void start();
        void stop();
        void addHandler(WsjcppLightWebHttpHandlerBase *pHandler);

    private:
        void checkAndRestartWorkers();
        void stopAndRemoveWorkers();
        std::string readAddress(int nSockFd);
        std::string TAG;
        WsjcppLightWebDequeHttpRequests *m_pDeque;
        bool m_bStop;

        int m_nMaxWorkers;
        int m_nPort;
        std::vector<WsjcppLightWebHttpHandlerBase *> *m_pVHandlers;
        std::vector<WsjcppLightWebHttpThreadWorker *> m_vWorkers;

        int m_nSockFd;
        struct sockaddr_in m_serverAddress;
        pthread_t m_serverThread;
};

#endif // WSJCPP_LIGHT_WEB_SERVER_H


