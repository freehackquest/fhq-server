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
#include <light_http_handlers.h>
#include <thread>

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
