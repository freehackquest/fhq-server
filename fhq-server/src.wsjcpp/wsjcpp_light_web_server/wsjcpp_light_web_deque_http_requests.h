#ifndef WSJCPP_LIGHT_WEB_DEQUE_HTTP_REQUESTS_H
#define WSJCPP_LIGHT_WEB_DEQUE_HTTP_REQUESTS_H

#include <string>
#include <deque>
#include <mutex>

#include "wsjcpp_light_web_http_request.h"

// ---------------------------------------------------------------------

class WSJCppLightWebDequeHttpRequests {
    public:
        WSJCppLightWebDequeHttpRequests();
        WSJCppLightWebHttpRequest *popRequest();
        void pushRequest(WSJCppLightWebHttpRequest *pRequest);
        void cleanup();

    private:
        std::string TAG;

        std::mutex m_mtxDequeRequests;
        std::mutex m_mtxWaiterRequests;
        std::deque<WSJCppLightWebHttpRequest *> m_dequeRequests;
};

#endif // WSJCPP_LIGHT_WEB_DEQUE_HTTP_REQUESTS_H


