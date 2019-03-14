#ifndef LIGHT_HTTP_THREAD_WORKER_H
#define LIGHT_HTTP_THREAD_WORKER_H

#include <light_http_deque_requests.h>
#include <light_http_handlers.h>

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

#endif // LIGHT_HTTP_SERVER_H
