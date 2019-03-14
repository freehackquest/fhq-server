#include "light_http_deque_requests.h"
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
#include <fs.h>

// ----------------------------------------------------------------------

LightHttpRequest *LightHttpDequeRequests::popRequest(){
	std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
	LightHttpRequest *pRequest = nullptr;
	int nSize = m_dequeRequests.size();
	if (nSize > 0) {
		pRequest = m_dequeRequests.back();
		m_dequeRequests.pop_back();
	}
	return pRequest;
}

// ----------------------------------------------------------------------

void LightHttpDequeRequests::pushRequest(LightHttpRequest *pRequest){
	std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
	if (m_dequeRequests.size() > 20) {
		Log::warn(TAG, " deque more than " + std::to_string(m_dequeRequests.size()));
	}
	m_dequeRequests.push_front(pRequest);
}

// ----------------------------------------------------------------------

void LightHttpDequeRequests::cleanup(){
	std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
	while (m_dequeRequests.size() > 0) {
		delete m_dequeRequests.back();
		m_dequeRequests.pop_back();
	}
}

// ----------------------------------------------------------------------