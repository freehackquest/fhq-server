#ifndef LIGHT_HTTP_RESPONSE_H
#define LIGHT_HTTP_RESPONSE_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <map>

class LightHttpResponse {
	public:
        static std::map<int, std::string> *g_mapReponseDescription;

        // enum for http responses
        static std::string RESP_OK;
        static std::string RESP_BAD_REQUEST;
		static std::string RESP_FORBIDDEN;
        static std::string RESP_NOT_FOUND;
        static std::string RESP_PAYLOAD_TOO_LARGE;
		static std::string RESP_INTERNAL_SERVER_ERROR;
		static std::string RESP_NOT_IMPLEMENTED;
        
        LightHttpResponse(int nSockFd);

        LightHttpResponse &ok();
        LightHttpResponse &badRequest();
        LightHttpResponse &forbidden();
        LightHttpResponse &notFound();
        LightHttpResponse &payloadTooLarge();
        LightHttpResponse &internalServerError();
        LightHttpResponse &notImplemented();

        LightHttpResponse &noCache();
		LightHttpResponse &cacheSec(int nCacheSec);

        void sendText(const std::string &sBody);
        void sendEmpty();
        void sendOptions(const std::string &sOptions);
        void sendDontUnderstand();

    private:
        std::string prepareHeaders(int nLength);
        
        std::string TAG;

        int m_nSockFd;
		bool m_bClosed;
        int m_nResponseCode;
        std::string m_sDataType;
        std::string m_sCacheControl;
		std::string m_sLastModified;
};

#endif // LIGHT_HTTP_RESPONSE_H
