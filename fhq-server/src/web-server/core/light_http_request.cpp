#include "light_http_request.h"
#include <logger.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>
#include <fstream>
#include <regex>        // regex, sregex_token_iterator
#include <stdio.h>
#include <math.h>
#include <fs.h>
#include <ts.h>
#include <str.h>
#include <light_http_response.h>

// ----------------------------------------------------------------------

LightHttpRequest::LightHttpRequest(int nSockFd, const std::string &sAddress) {
	m_nSockFd = nSockFd;
	m_sAddress = sAddress;
	m_bClosed = false;
	m_sRequest = "";
	m_nParserState = EnumParserState::START;
	TAG = "LightHttpRequest";
	long nSec = TS::currentTime_seconds();
	m_sLastModified = TS::formatTimeForWeb(nSec);
	m_nContentLength = 0;
}

// ----------------------------------------------------------------------

int LightHttpRequest::sockFd(){
	return m_nSockFd;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestType(){
	return m_sRequestType;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestPath(){
	return m_sRequestPath;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestHttpVersion(){
	return m_sRequestHttpVersion;
}

std::map<std::string,std::string> &LightHttpRequest::requestQueryParams() {
	return m_sRequestQueryParams;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::address(){
	return m_sAddress;
}

// ----------------------------------------------------------------------

void LightHttpRequest::appendRecieveRequest(const std::string &sRequestPart) {
	m_sRequest += sRequestPart;
	const std::string sContentLengthPrefix = "content-length:";
	if (m_nParserState == EnumParserState::START) {
		m_vHeaders.clear();
		// Log::info(TAG, "START \n>>>\n" + m_sRequest + "\n<<<\n");

		std::istringstream f(m_sRequest);
		std::string sLine = "";
		int nSize = 0;
		bool bHeadersEnded = false;
		while (getline(f, sLine, '\n')) {
			nSize += sLine.length() + 1;
			Str::trim(sLine);
			// Log::info(TAG, "Line: {" + sLine + "}, size=" + std::to_string(sLine.length()));
			if (sLine.length() == 0) {
				bHeadersEnded = true;
				break;
			}
			m_vHeaders.push_back(sLine);

			Str::to_lower(sLine);
			if (!sLine.compare(0, sContentLengthPrefix.size(), sContentLengthPrefix)) {
				m_nContentLength = atoi(sLine.substr(sContentLengthPrefix.size()).c_str());
				// Log::warn(TAG, "Content-Length: " + std::to_string(m_nContentLength));
			}
		}

		if (bHeadersEnded) {
			if (m_vHeaders.size() > 0) {
				this->parseFirstLine(m_vHeaders[0]);
			}
			m_sRequest.erase(0, nSize);
			// Log::info(TAG, "AFTER ERASE \n>>>\n" + m_sRequest + "\n<<<\n");
			m_nParserState = EnumParserState::BODY;
		} else {
			// Log::info(TAG, "Not ended");
		}
	}
	
	if (m_nParserState == EnumParserState::BODY && m_sRequest.length() == m_nContentLength) {
		m_nParserState = EnumParserState::ENDED;
		m_sRequestBody = m_sRequest;
	}
}

// ----------------------------------------------------------------------

bool LightHttpRequest::isEnoughAppendReceived() {
	return m_nParserState == EnumParserState::ENDED;
}

// ----------------------------------------------------------------------

void LightHttpRequest::parseFirstLine(const std::string &sHeader) {
	if (sHeader.size() > 0) {
		std::istringstream f(sHeader);
		std::vector<std::string> params;
		std::string s;
		while (getline(f, s, ' ')) {
			params.push_back(s);
		}
		if(params.size() > 0){
			m_sRequestType = params[0];
		}

		if(params.size() > 1){
			m_sRequestPath = params[1];
		}

		// TODO m_sRequestPath - need split by ? if exists
		if(params.size() > 2){
			m_sRequestHttpVersion = params[2];
		}
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
			m_sRequestQueryParams[sName] = sValue; // wrong use map for params
		}
	}
}

// ----------------------------------------------------------------------
/*
void LightHttpRequest::parseRequest(const std::string &sRequest){
    std::istringstream f(sRequest);
	std::string firstLine = "";
    if (getline(f, firstLine, '\n')) {
        // nothing
    }

	if (firstLine.size() > 0) {
		std::istringstream f(firstLine);
		std::vector<std::string> params;
		std::string s;
		while (getline(f, s, ' ')) {
			params.push_back(s);
		}
		if(params.size() > 0){
			m_sRequestType = params[0];
		}

		if(params.size() > 1){
			m_sRequestPath = params[1];
		}

		// TODO m_sRequestPath - need split by ? if exists
		if(params.size() > 2){
			m_sRequestHttpVersion = params[2];
		}
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
			m_sRequestQueryParams[sName] = sValue;
		}
	}
}
*/