#ifndef HTTP_HANDLER_WEB_PUBLIC_FOLDER_H
#define HTTP_HANDLER_WEB_PUBLIC_FOLDER_H

#include <wsjcpp_light_web_server.h>

class HttpHandlerWebPublicFolder : WSJCppLightWebHttpHandlerBase {
    public:
        HttpHandlerWebPublicFolder(const std::string &sWebFolder);
        virtual bool canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        std::string m_sWebPublicFolder;
};

#endif // HTTP_HANDLER_WEB_PUBLIC_FOLDER_H
