#ifndef HTTP_HANDLER_WEB_USER_FOLDER_H
#define HTTP_HANDLER_WEB_USER_FOLDER_H

#include <wsjcpp_light_web_server.h>

class HttpHandlerWebUserFolder : WsjcppLightWebHttpHandlerBase {
    public:
        HttpHandlerWebUserFolder(const std::string &sWebFolder);
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        std::string m_sWebFolder;
};

#endif // HTTP_HANDLER_WEB_USER_FOLDER_H
