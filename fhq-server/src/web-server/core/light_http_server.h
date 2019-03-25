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
#include <light_http_request.h>
#include <light_http_response.h>
#include <light_http_handler_base.h>

// ---------------------------------------------------------------------

class LightHttpDequeRequests {
	public:
        LightHttpRequest *popRequest();
		void pushRequest(LightHttpRequest *pRequest);
		void cleanup();

	private:
		std::string TAG;

		std::mutex m_mtxDequeRequests;
		std::deque<LightHttpRequest *> m_dequeRequests;
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
