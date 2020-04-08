#include "wsjcpp_light_web_deque_http_requests.h"

#include <wsjcpp_core.h>

// ----------------------------------------------------------------------
// WsjcppLightWebDequeHttpRequests

WsjcppLightWebDequeHttpRequests::WsjcppLightWebDequeHttpRequests() {
    TAG = "WsjcppLightWebDequeHttpRequests";
}

// ----------------------------------------------------------------------

WsjcppLightWebHttpRequest *WsjcppLightWebDequeHttpRequests::popRequest() {
    if (m_dequeRequests.size() == 0) {
        m_mtxWaiterRequests.lock();
    }
    std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
    WsjcppLightWebHttpRequest *pRequest = nullptr;
    int nSize = m_dequeRequests.size();
    if (nSize > 0) {
        pRequest = m_dequeRequests.back();
        m_dequeRequests.pop_back();
    }
    return pRequest;
}

// ----------------------------------------------------------------------

void WsjcppLightWebDequeHttpRequests::pushRequest(WsjcppLightWebHttpRequest *pRequest) {
    {
        std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
        if (m_dequeRequests.size() > 20) {
            WsjcppLog::warn(TAG, " deque more than " + std::to_string(m_dequeRequests.size()));
        }
        m_dequeRequests.push_front(pRequest);
    }
    
    if (m_dequeRequests.size() == 1) {
        m_mtxWaiterRequests.unlock();   
    }
}

// ----------------------------------------------------------------------

void WsjcppLightWebDequeHttpRequests::cleanup() {
    std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
    while (m_dequeRequests.size() > 0) {
        delete m_dequeRequests.back();
        m_dequeRequests.pop_back();
    }
}
