#ifndef LIGHT_HTTP_HANDLERS_H
#define LIGHT_HTTP_HANDLERS_H

#include <map>
#include <string>
#include <vector>
#include <light_http_handler_base.h>
#include <light_http_request.h>

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


#endif // LIGHT_HTTP_HANDLERS_H