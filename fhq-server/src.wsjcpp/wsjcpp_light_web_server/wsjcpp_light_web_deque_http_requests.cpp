#include "wsjcpp_light_web_deque_http_requests.h"

#include <wsjcpp_core.h>

// ----------------------------------------------------------------------
// WSJCppLightWebDequeHttpRequests

WSJCppLightWebDequeHttpRequests::WSJCppLightWebDequeHttpRequests() {
    TAG = "WSJCppLightWebDequeHttpRequests";
}

// ----------------------------------------------------------------------

WSJCppLightWebHttpRequest *WSJCppLightWebDequeHttpRequests::popRequest() {
    if (m_dequeRequests.size() == 0) {
        m_mtxWaiterRequests.lock();
    }
    std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
    WSJCppLightWebHttpRequest *pRequest = nullptr;
    int nSize = m_dequeRequests.size();
    if (nSize > 0) {
        pRequest = m_dequeRequests.back();
        m_dequeRequests.pop_back();
    }
    return pRequest;
}

// ----------------------------------------------------------------------

void WSJCppLightWebDequeHttpRequests::pushRequest(WSJCppLightWebHttpRequest *pRequest) {
    {
        std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
        if (m_dequeRequests.size() > 20) {
            WSJCppLog::warn(TAG, " deque more than " + std::to_string(m_dequeRequests.size()));
        }
        m_dequeRequests.push_front(pRequest);
    }
    
    if (m_dequeRequests.size() == 1) {
        m_mtxWaiterRequests.unlock();   
    }
}

// ----------------------------------------------------------------------

void WSJCppLightWebDequeHttpRequests::cleanup() {
    std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
    while (m_dequeRequests.size() > 0) {
        delete m_dequeRequests.back();
        m_dequeRequests.pop_back();
    }
}
