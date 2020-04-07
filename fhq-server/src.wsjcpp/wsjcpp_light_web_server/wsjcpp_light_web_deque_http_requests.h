#ifndef WSJCPP_LIGHT_WEB_DEQUE_HTTP_REQUESTS_H
#define WSJCPP_LIGHT_WEB_DEQUE_HTTP_REQUESTS_H

#include <string>
#include <deque>
#include <mutex>

#include "wsjcpp_light_web_http_request.h"

// ---------------------------------------------------------------------

class WsjcppLightWebDequeHttpRequests {
    public:
        WsjcppLightWebDequeHttpRequests();
        WsjcppLightWebHttpRequest *popRequest();
        void pushRequest(WsjcppLightWebHttpRequest *pRequest);
        void cleanup();

    private:
        std::string TAG;

        std::mutex m_mtxDequeRequests;
        std::mutex m_mtxWaiterRequests;
        std::deque<WsjcppLightWebHttpRequest *> m_dequeRequests;
};

#endif // WSJCPP_LIGHT_WEB_DEQUE_HTTP_REQUESTS_H


