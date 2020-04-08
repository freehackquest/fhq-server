#ifndef HTTP_HANDLER_WEB_PUBLIC_FOLDER_H
#define HTTP_HANDLER_WEB_PUBLIC_FOLDER_H

#include <wsjcpp_light_web_server.h>

class HttpHandlerWebPublicFolder : WsjcppLightWebHttpHandlerBase {
    public:
        HttpHandlerWebPublicFolder(const std::string &sWebFolder);
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        std::string m_sWebPublicFolder;
};

#endif // HTTP_HANDLER_WEB_PUBLIC_FOLDER_H
