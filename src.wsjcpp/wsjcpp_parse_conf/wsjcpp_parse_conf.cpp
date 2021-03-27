
#include "wsjcpp_parse_conf.h"
#include <wsjcpp_core.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

// ---------------------------------------------------------------------
// WsjcppParseConf

WsjcppParseConf::WsjcppParseConf(const std::string &sFilepathConf) {
    TAG = "WsjcppParseConf";
    m_sFilepathConf = sFilepathConf;
}

// ---------------------------------------------------------------------

bool WsjcppParseConf::load() {
    std::ifstream isConfigFile( m_sFilepathConf );
    int nLineNumber = 0;
    for (std::string sLine; getline(isConfigFile, sLine);) {
        nLineNumber++;
        std::string sOrigLine = sLine;
        std::size_t nFoundComment = sLine.find("#");
        
        if (nFoundComment != std::string::npos) {
            // remove all after #
            sLine.erase(sLine.begin() + nFoundComment, sLine.end());
        }

        WsjcppCore::trim(sLine);
        if (sLine == "") { // skip empty strings
            continue;
        }
        
        // std::cout << "Line (" << nLineNumber << "): [" << sLine << "]" << std::endl;
        
        std::size_t nFoundEqualChar = sLine.find("=");
        if (nFoundEqualChar != std::string::npos) {
            // split name of param and value
            std::string sParamName = sLine;
            std::string sParamValue = sLine;
            
            sParamName.erase (sParamName.begin() + nFoundEqualChar, sParamName.end());
            sParamValue.erase (sParamValue.begin(), sParamValue.begin() + nFoundEqualChar + 1);
            WsjcppCore::trim(sParamName);
            WsjcppCore::trim(sParamValue);
            
            // std::cout << " [" << sParamName << "]  => [" << sParamValue << "]" << std::endl;
            
            if (m_mapConfigValues.count(sParamName)) {
                WsjcppLog::warn(TAG, "Ignoring duplicate of option line(" + std::to_string(nLineNumber) + ") in config: " + m_sFilepathConf);
            } else {
                m_mapConfigValues.insert(std::pair<std::string,std::string>(sParamName, sParamValue));    
            }
        } else {
            WsjcppLog::warn(TAG, "Ignoring invalid line(" + std::to_string(nLineNumber) + ") in config: " + m_sFilepathConf);
        }
        
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppParseConf::has(const std::string &sParamName) {
    return m_mapConfigValues.count(sParamName);
}

// ---------------------------------------------------------------------

std::string WsjcppParseConf::getStringValue(const std::string &sParamName, const std::string &defaultValue) {
    std::string sResult = defaultValue;
    if (m_mapConfigValues.count(sParamName)) {
        sResult = m_mapConfigValues.at(sParamName);
    } else {
        WsjcppLog::warn(TAG, sParamName + " - not found in " + m_sFilepathConf + "\n\t Will be used default value: " + defaultValue);
    }
    return sResult;
}

// ---------------------------------------------------------------------

int WsjcppParseConf::getIntValue(const std::string &sParamName, int defaultValue) {
    int nResult = defaultValue;
    if (m_mapConfigValues.count(sParamName)) {
        std::string sParamValue = m_mapConfigValues.at(sParamName);
        std::istringstream isBuffer(sParamValue);
        isBuffer >> nResult;
    } else {
        WsjcppLog::warn(TAG, sParamName + " - not found in " + m_sFilepathConf + "\n\t Will be used default value: " + std::to_string(defaultValue));
    }
    return nResult;
}

// ---------------------------------------------------------------------

bool WsjcppParseConf::getBoolValue(const std::string &sParamName, bool defaultValue) {
    bool bResult = defaultValue;

    if (m_mapConfigValues.count(sParamName)) {
        std::string sParamValue = m_mapConfigValues.at(sParamName);
        std::transform(sParamValue.begin(), sParamValue.end(), sParamValue.begin(), ::tolower);
        bResult = (sParamValue == "yes");
    } else {
        WsjcppLog::warn(TAG, sParamName + " - not found in " + m_sFilepathConf + "\n\t Will be used default value: " + (defaultValue ? "yes" : "no"));
    }
    return bResult;
}

// ---------------------------------------------------------------------

std::map<std::string,std::string> WsjcppParseConf::getValues() {
    return m_mapConfigValues;
}

// ---------------------------------------------------------------------
