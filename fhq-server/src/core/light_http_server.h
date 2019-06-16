#ifndef LIGHT_HTTP_SERVER_H
#define LIGHT_HTTP_SERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <map>
#include <deque>
#include <mutex>
#include <vector>

// ---------------------------------------------------------------------

class LightHttpResponse {
    public:
        static std::map<int, std::string> *g_mapReponseDescription;

        // enum for http responses
        static std::string RESP_OK;
        static std::string RESP_BAD_REQUEST;
        static std::string RESP_FORBIDDEN;
        static std::string RESP_NOT_FOUND;
        static std::string RESP_PAYLOAD_TOO_LARGE;
        static std::string RESP_INTERNAL_SERVER_ERROR;
        static std::string RESP_NOT_IMPLEMENTED;
        
        LightHttpResponse(int nSockFd);

        LightHttpResponse &ok();
        LightHttpResponse &badRequest();
        LightHttpResponse &forbidden();
        LightHttpResponse &notFound();
        LightHttpResponse &payloadTooLarge();
        LightHttpResponse &internalServerError();
        LightHttpResponse &notImplemented();

        LightHttpResponse &noCache();
        LightHttpResponse &cacheSec(int nCacheSec);

        void sendText(const std::string &sBody);
        void sendEmpty();
        void sendOptions(const std::string &sOptions);
        void sendDontUnderstand();
        void sendFile(const std::string &sFilePath);
        void sendBuffer(const std::string &sFilePath, const char *pBuffer, const int nBufferSize);

    private:
        std::string prepareHeaders(int nLength);
        std::string detectTypeOfFile(const std::string &sFilePath);

        std::string TAG;

        int m_nSockFd;
        bool m_bClosed;
        int m_nResponseCode;
        std::string m_sDataType;
        std::string m_sCacheControl;
        std::string m_sLastModified;
};

// ---------------------------------------------------------------------

class LightHttpRequest {
    public:
        LightHttpRequest(
            int nSockFd,
            const std::string &sAddress
        );
        ~LightHttpRequest() {};

        int sockFd();
        void appendRecieveRequest(const std::string &sRequestPart);
        bool isEnoughAppendReceived();
        
        std::string address();
        std::string requestType();
        std::string requestPath();
        std::string requestBody();
        std::string requestHttpVersion();
        std::map<std::string,std::string> &requestQueryParams();

    private:
        std::string TAG;

        void parseFirstLine(const std::string &sHeader);

        enum EnumParserState {
            START,
            BODY,
            ENDED
        };
        int m_nSockFd;
        bool m_bClosed;
        EnumParserState m_nParserState;
        std::vector<std::string> m_vHeaders;
        int m_nContentLength;
        std::string m_sRequest;
        std::string m_sAddress;
        std::string m_sRequestType;
        std::string m_sRequestPath;
        std::string m_sRequestBody;
        std::map<std::string,std::string> m_sRequestQueryParams; // wrong use map for params
        std::string m_sRequestHttpVersion;

        std::string m_sResponseCacheControl;
        std::string m_sLastModified;
};

// ---------------------------------------------------------------------

class LightHttpDequeRequests {
    public:
        LightHttpRequest *popRequest();
        void pushRequest(LightHttpRequest *pRequest);
        void cleanup();

    private:
        std::string TAG;

        std::mutex m_mtxDequeRequests;
        std::mutex m_mtxWaiterRequests;
        std::deque<LightHttpRequest *> m_dequeRequests;
};

// ---------------------------------------------------------------------

class LightHttpHandlerBase {
    public:
        LightHttpHandlerBase(const std::string &sName);
        const std::string &name();
        virtual bool canHandle(const std::string &sWorkerId, LightHttpRequest *pRequest) = 0;
        virtual bool handle(const std::string &sWorkerId, LightHttpRequest *pRequest) = 0;

    private:
        std::string m_sName;
};

// ---------------------------------------------------------------------

class LightHttpHandlers {
    public:
        LightHttpHandlers();
        void add(LightHttpHandlerBase *pHandler);
        void remove(const std::string &sName);
        bool find(const std::string &sName, LightHttpHandlerBase *pHandler);
        bool handle(const std::string &sWorkerId, LightHttpRequest *pRequest);
    private:
        std::string TAG;
        std::vector<LightHttpHandlerBase *> m_pHandlers;
};

// ---------------------------------------------------------------------

class LightHttpThreadWorker {
    public:

        LightHttpThreadWorker(const std::string &sName, LightHttpDequeRequests *pDeque, LightHttpHandlers *pHandlers);

        void start();
        void stop();
        void run();        
    private:
        std::string TAG;
        std::string m_sName;
        LightHttpDequeRequests *m_pDeque;
        LightHttpHandlers *m_pHandlers;
        bool m_bStop;
        pthread_t m_serverThread;
};

// ---------------------------------------------------------------------

class LightHttpServer {
    public:

        LightHttpServer();
        void setPort(int nPort);
        void setMaxWorkers(int nMaxWorkers);
        void startSync();
        void start();
        void stop();

        LightHttpHandlers *handlers();

    private:
        std::string TAG;
        LightHttpDequeRequests *m_pDeque;
        LightHttpHandlers *m_pHandlers;
        bool m_bStop;

        int m_nMaxWorkers;
        int m_nPort;
        std::vector<LightHttpThreadWorker *> m_vWorkers;

        int m_nSockFd;
        struct sockaddr_in m_serverAddress;
        pthread_t m_serverThread;
};

#endif // LIGHT_HTTP_SERVER_H
