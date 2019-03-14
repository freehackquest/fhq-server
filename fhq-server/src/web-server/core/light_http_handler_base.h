#ifndef LIGHT_HTTP_HANDLER_BASE_H
#define LIGHT_HTTP_HANDLER_BASE_H

#include <map>
#include <string>
#include <vector>
#include <light_http_request.h>

// ---------------------------------------------------------------------

class LightHttpHandlerBase {
	public:
        LightHttpHandlerBase(const std::string &sName);
        const std::string &name();
		virtual bool canHandle(const std::string &sWorkerId, LightHttpRequest *pRequest) = 0;
        virtual bool handle(const std::string &sWorkerId, LightHttpRequest *pRequest) = 0;

    private:
        std::string m_sName;
};

#endif // LIGHT_HTTP_HANDLER_BASE_H