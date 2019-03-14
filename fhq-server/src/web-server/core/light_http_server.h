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
#include <light_http_request.h>
#include <light_http_handlers.h>
#include <light_http_deque_requests.h>
#include <light_http_thread_worker.h>

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
