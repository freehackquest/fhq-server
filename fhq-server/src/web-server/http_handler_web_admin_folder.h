#ifndef HTTP_HANDLER_WEB_ADMIN_FOLDER_H
#define HTTP_HANDLER_WEB_ADMIN_FOLDER_H

#include <wsjcpp_light_web_server.h>

class HttpHandlerWebAdminFolder : WSJCppLightWebHttpHandlerBase {
    public:
        HttpHandlerWebAdminFolder(const std::string &sWebFolder);
        virtual bool canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        std::string m_sWebFolder;
};

#endif // HTTP_HANDLER_WEB_ADMIN_FOLDER_H
