#ifndef WSJCPP_LIGHT_WEB_HTTP_HANDLER_WEB_FOLDER_H
#define WSJCPP_LIGHT_WEB_HTTP_HANDLER_WEB_FOLDER_H

#include <wsjcpp_light_web_server.h>

class WsjcppLightWebHttpHandlerWebFolder : public WsjcppLightWebHttpHandlerBase {
    public:
        WsjcppLightWebHttpHandlerWebFolder(const std::string &sPrefixPath, const std::string &sWebFolder);
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        std::string m_sPrefixPath;
        std::string m_sWebFolder;
};

#endif // WSJCPP_LIGHT_WEB_HTTP_HANDLER_WEB_FOLDER_H