#include "wsjcpp_light_web_http_request.h"

#include <sstream>
#include <wsjcpp_core.h>

// ----------------------------------------------------------------------

WSJCppLightWebHttpRequestQueryValue::WSJCppLightWebHttpRequestQueryValue(const std::string &sName, const std::string &sValue) {
    m_sName = sName;
    m_sValue = sValue;
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpRequestQueryValue::getName() const {
    return m_sName;
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpRequestQueryValue::getValue() const {
    return m_sValue;
}

// ----------------------------------------------------------------------
// WSJCppLightWebHttpRequest

WSJCppLightWebHttpRequest::WSJCppLightWebHttpRequest(int nSockFd, const std::string &sAddress) {
    TAG = "WSJCppLightWebHttpRequest";
    m_sUniqueId = WSJCppCore::createUuid();
    m_nSockFd = nSockFd;
    m_sAddress = sAddress;
    m_bClosed = false;
    m_sRequest = "";
    m_nParserState = EnumParserState::START;
    long nSec = WSJCppCore::currentTime_seconds();
    m_sLastModified = WSJCppCore::formatTimeForWeb(nSec);
    m_nContentLength = 0;
}

// ----------------------------------------------------------------------

int WSJCppLightWebHttpRequest::getSockFd() const {
    return m_nSockFd;
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpRequest::getUniqueId() const {
    return m_sUniqueId;
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpRequest::getRequestType() const {
    return m_sRequestType;
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpRequest::getRequestPath() const {
    return m_sRequestPath;
}

std::string WSJCppLightWebHttpRequest::getRequestBody() const {
    return m_sRequestBody;
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpRequest::getRequestHttpVersion() const {
    return m_sRequestHttpVersion;
}

// ----------------------------------------------------------------------

const std::vector<WSJCppLightWebHttpRequestQueryValue> &WSJCppLightWebHttpRequest::getRequestQueryParams() {
    return m_vRequestQueryParams;
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpRequest::getAddress() const {
    return m_sAddress;
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpRequest::appendRecieveRequest(const std::string &sRequestPart) {
    m_sRequest += sRequestPart;
    const std::string sContentLengthPrefix = "content-length:";
    if (m_nParserState == EnumParserState::START) {
        m_vHeaders.clear();
        // WSJCppLog::info(TAG, "START \n>>>\n" + m_sRequest + "\n<<<\n");

        std::istringstream f(m_sRequest);
        std::string sLine = "";
        int nSize = 0;
        bool bHeadersEnded = false;
        while (getline(f, sLine, '\n')) {
            nSize += sLine.length() + 1;
            WSJCppCore::trim(sLine);
            // WSJCppLog::info(TAG, "Line: {" + sLine + "}, size=" + std::to_string(sLine.length()));
            if (sLine.length() == 0) {
                bHeadersEnded = true;
                break;
            }
            m_vHeaders.push_back(sLine);

            WSJCppCore::to_lower(sLine);
            if (!sLine.compare(0, sContentLengthPrefix.size(), sContentLengthPrefix)) {
                m_nContentLength = atoi(sLine.substr(sContentLengthPrefix.size()).c_str());
                // WSJCppLog::warn(TAG, "Content-Length: " + std::to_string(m_nContentLength));
            }
        }

        if (bHeadersEnded) {
            if (m_vHeaders.size() > 0) {
                this->parseFirstLine(m_vHeaders[0]);
            }
            m_sRequest.erase(0, nSize);
            // WSJCppLog::info(TAG, "AFTER ERASE \n>>>\n" + m_sRequest + "\n<<<\n");
            m_nParserState = EnumParserState::BODY;
        } else {
            // WSJCppLog::info(TAG, "Not ended");
        }
    }
    
    if (m_nParserState == EnumParserState::BODY && m_sRequest.length() >= m_nContentLength) {
        m_nParserState = EnumParserState::ENDED;
        m_sRequestBody = m_sRequest.substr(0, m_nContentLength);
    }
}

// ----------------------------------------------------------------------

bool WSJCppLightWebHttpRequest::isEnoughAppendReceived() const {
    return m_nParserState == EnumParserState::ENDED;
}

// ----------------------------------------------------------------------

void WSJCppLightWebHttpRequest::parseFirstLine(const std::string &sHeader) {
    if (sHeader.size() > 0) {
        std::istringstream f(sHeader);
        std::vector<std::string> params;
        std::string s;
        while (getline(f, s, ' ')) {
            params.push_back(s);
        }
        if (params.size() > 0) {
            m_sRequestType = params[0];
        }

        if (params.size() > 1) {
            m_sRequestPath = params[1];
        }

        // TODO m_sRequestPath - need split by ? if exists
        if (params.size() > 2) {
            m_sRequestHttpVersion = params[2];
        }
    }

    if (m_sRequestPath.length() == 0) {
        m_sRequestPath = "/";
    }
    m_sRequestPath = WSJCppCore::doNormalizePath(m_sRequestPath);

    // TODO url path encoding
    while (m_sRequestPath.length() > 2 && m_sRequestPath.substr(0,2) == "..") {
        m_sRequestPath = WSJCppCore::doNormalizePath("/erase/" + m_sRequestPath);
    }

    // parse query
    std::size_t nFound = m_sRequestPath.find("?");
      if (nFound != std::string::npos) {
        std::string sQuery = m_sRequestPath.substr(nFound+1);
        m_sRequestPath = m_sRequestPath.substr(0, nFound);
        std::istringstream f(sQuery);
        std::string sParam;
        while (getline(f, sParam, '&')) {
            std::size_t nFound2 = sParam.find("=");
            std::string sValue = sParam.substr(nFound2+1);
            std::string sName = sParam.substr(0, nFound2);
            m_vRequestQueryParams.push_back(WSJCppLightWebHttpRequestQueryValue(
                this->decodeURIElement(sName), 
                this->decodeURIElement(sValue)
            ));
        }
    }
}

// ----------------------------------------------------------------------

std::string WSJCppLightWebHttpRequest::decodeURIElement(const std::string &sElement) { // TODO move to WSJCppCore
    WSJCppLog::warn(TAG, "TODO Implement decodeURIElement");
    return sElement;
}
