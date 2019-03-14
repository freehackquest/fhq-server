#ifndef LIGHT_HTTP_DEQUE_REQUESTS_H
#define LIGHT_HTTP_DEQUE_REQUESTS_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mutex>
#include <deque>
#include <light_http_request.h>

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

#endif // LIGHT_HTTP_DEQUE_REQUESTS_H