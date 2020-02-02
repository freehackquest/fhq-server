#include "fallen.h"
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <ctime>
#include <math.h>
#include <chrono>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <cstdint>
#include <unistd.h>

// ---------------------------------------------------------------------

FallenHelpParseArg::FallenHelpParseArg(const std::string &sName, const std::string &sAlias, FallenHelpParseArgType nType, const std::string &sDescription) {
     m_sName = sName;
     m_sAlias = sAlias;
     m_sDescription = sDescription;
     m_bIsOption = (nType == FallenHelpParseArgType::PARAMETER);
}

// ---------------------------------------------------------------------

FallenHelpParseArg::FallenHelpParseArg() {
     m_sName = "";
     m_sAlias = "";
     m_sDescription = "";
     m_bIsOption = false;
 }

// ---------------------------------------------------------------------

std::string FallenHelpParseArg::name() {
     return m_sName;
}

// ---------------------------------------------------------------------

std::string FallenHelpParseArg::alias() {
     return m_sAlias;
}

// ---------------------------------------------------------------------

std::string FallenHelpParseArg::description() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

bool FallenHelpParseArg::isOption() {
    return m_bIsOption;
}

// ---------------------------------------------------------------------

FallenHelpParseArgs::FallenHelpParseArgs(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        m_vArgs.push_back(std::string(argv[i]));
    }
}

// ---------------------------------------------------------------------

void FallenHelpParseArgs::setAppName(const std::string &sAppName) {
    m_sAppName = sAppName;
}

// ---------------------------------------------------------------------

void FallenHelpParseArgs::setAppVersion(const std::string &sAppVersion) {
    m_sAppVersion = sAppVersion;
}

// ---------------------------------------------------------------------

std::string FallenHelpParseArgs::option(const std::string &sName) {
    FallenHelpParseArg *pHpa = NULL;
    for (unsigned int i = 0; i < m_vHelpArgs.size(); i++) {
        if (m_vHelpArgs[i]->name() == sName) {
            pHpa = m_vHelpArgs[i];
            break;
        }
    }
    if (pHpa == NULL) {
        std::cout << "Error: Not defined attribute \n";
        // Log::err(TAG, "Not defined attribute");
        return "";
    }

    if (!pHpa->isOption()) {
        std::cout << "Error: It's not option \n";
        return "";
    }

    std::vector<std::string>::iterator it1 = std::find(m_vArgs.begin(), m_vArgs.end(), pHpa->name());
    if (it1 != m_vArgs.end() && std::next(it1, 1) != m_vArgs.end()) {
        return * std::next(it1, 1);
    }

    std::vector<std::string>::iterator it2 = std::find(m_vArgs.begin(), m_vArgs.end(), pHpa->alias());
    if (it2 != m_vArgs.end() && std::next(it2, 1) != m_vArgs.end()) {
        return * std::next(it2, 1);
    }
    return "";
}

// ---------------------------------------------------------------------

bool FallenHelpParseArgs::has(const std::string &sName) {
    std::vector<std::string> m_vAliases;
    m_vAliases.push_back(sName);
    FallenHelpParseArg *pHpa = NULL;
    for (unsigned int i = 0; i < m_vHelpArgs.size(); i++) {
        if (m_vHelpArgs[i]->name() == sName) {
            pHpa = m_vHelpArgs[i];
            m_vAliases.push_back(pHpa->alias());
        }
    }
    if (pHpa == NULL) {
        std::cout << "Error: Not defined attribute for '" << sName << "'\n";
        // Log::err(TAG, "Not defined attribute");
        return false;
    }

    for (unsigned int i = 0; i < m_vAliases.size(); i++) {
        std::string sAlias = m_vAliases[i];
        std::vector<std::string>::iterator it = std::find(m_vArgs.begin(), m_vArgs.end(), sAlias);
        if (it != m_vArgs.end()) {
            // std::cout << "Found " + sAlias << ", " << pHpa->name() << ", " << (pHpa->isOption() ? "option" : "not option") <<  " \n";
            
        }
        if (it != m_vArgs.end() && !pHpa->isOption()) {
            return true;
        }

        if (pHpa->isOption() && it != m_vArgs.end() && std::next(it, 1) != m_vArgs.end()) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

void FallenHelpParseArgs::addHelp(const std::string &sName, const std::string &sAlias, FallenHelpParseArgType nType, const std::string &sDescription) {
    m_vHelpArgs.push_back(new FallenHelpParseArg(sName, sAlias, nType, sDescription));
}

// ---------------------------------------------------------------------

void FallenHelpParseArgs::printHelp() {
    // TODO calculate max size
    int nMaxSize = 0;
    for (unsigned int i = 0; i < m_vHelpArgs.size(); i++) {
        FallenHelpParseArg *pHpa = m_vHelpArgs[i];
        std::string sTmp = pHpa->name() + ", " + pHpa->alias();
        nMaxSize = std::max(nMaxSize, (int)sTmp.size());
    }
    nMaxSize += 4;

    // TODO:
    std::cout << "" << m_sAppName << " " << m_sAppVersion << "\n";
    std::cout << "Usage: " << m_vArgs.at(0) << "   [PARAM]\n";
    for (unsigned int i = 0; i < m_vHelpArgs.size(); i++) {
        FallenHelpParseArg *pHpa = m_vHelpArgs[i];
        std::string sTmp = pHpa->name() + ", " + pHpa->alias();
        int nIntent = nMaxSize - sTmp.size();

        std::cout << "\t " << sTmp;
        for (int x = 0; x < nIntent; x++) {
            std::cout << " ";
        }
        std::cout << pHpa->description() << "\n";
    }

    std::cout << "\n";
}

// ---------------------------------------------------------------------

bool FallenHelpParseArgs::checkArgs(std::string &sArgsErrors) {
    // TODO 
    return true;
}

// ---------------------------------------------------------------------
// Log

// Last log messages
std::deque<std::string> *g_LAST_LOG_MESSAGES = NULL;
std::mutex *g_LOG_MUTEX = NULL;
std::string Log::g_LOG_DIR = "./";
std::string Log::g_LOG_FILE = "";
std::string Log::g_PREFIX_LOG_FILE = "";
long Log::g_LOG_START_TIME = 0;

// ---------------------------------------------------------------------

void Log::doLogRotateUpdateFilename(bool bForce) {
    long t = WSJCppCore::currentTime_seconds();
    long nEverySeconds = 51000; // rotate log if started now or if time left more then 1 day
    if (g_LOG_START_TIME == 0 || t - g_LOG_START_TIME > nEverySeconds || bForce) {
        g_LOG_START_TIME = t;
        g_LOG_FILE = g_LOG_DIR + "/" + Log::g_PREFIX_LOG_FILE + "_" + WSJCppCore::formatTimeForFilename(g_LOG_START_TIME) + ".log";
    }
}

// ---------------------------------------------------------------------

void Log::info(const std::string & sTag, const std::string &sMessage) {
    WSJCppColorModifier def(WSJCppColorCode::FG_DEFAULT);
    Log::add(def, "INFO", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::err(const std::string & sTag, const std::string &sMessage) {
    WSJCppColorModifier red(WSJCppColorCode::FG_RED);
    Log::add(red, "ERR", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::throw_err(const std::string &sTag, const std::string &sMessage) {
    WSJCppColorModifier red(WSJCppColorCode::FG_RED);
    Log::add(red, "ERR", sTag, sMessage);
    throw std::runtime_error(sMessage);
}

// ---------------------------------------------------------------------

void Log::warn(const std::string & sTag, const std::string &sMessage) {
    WSJCppColorModifier yellow(WSJCppColorCode::FG_YELLOW);
    Log::add(yellow, "WARN",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::ok(const std::string &sTag, const std::string &sMessage) {
    WSJCppColorModifier green(WSJCppColorCode::FG_GREEN);
    Log::add(green, "OK", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::setLogDirectory(const std::string &sDirectoryPath) {
    Log::g_LOG_DIR = sDirectoryPath;
    Log::doLogRotateUpdateFilename(true);
}

// ---------------------------------------------------------------------

void Log::setPrefixLogFile(const std::string &sPrefixLogFile) {
    Log::g_PREFIX_LOG_FILE = sPrefixLogFile;
    Log::doLogRotateUpdateFilename(true);
}

// ---------------------------------------------------------------------

void Log::initGlobalVariables() {
    // create deque if not created
    if (g_LAST_LOG_MESSAGES == NULL) {
        g_LAST_LOG_MESSAGES = new std::deque<std::string>();
        // std::cout << Fallen::currentTime_logformat() + ", " + Fallen::threadId() + " Init last messages deque\r\n";
    }
    // create mutex if not created
    if (g_LOG_MUTEX == NULL) {
        g_LOG_MUTEX = new std::mutex();
        // std::cout << Fallen::currentTime_logformat() + ", " + Fallen::threadId() + " Init mutex for log\r\n";
    }
}

// ---------------------------------------------------------------------

nlohmann::json Log::getLastLogs() {
    Log::initGlobalVariables();
    std::lock_guard<std::mutex> lock(*g_LOG_MUTEX);
    nlohmann::json lastLogMessages = nlohmann::json::array();
    int len = g_LAST_LOG_MESSAGES->size();
    for (int i = 0; i < len; i++) {
        lastLogMessages.push_back(g_LAST_LOG_MESSAGES->at(i));
    }
    return lastLogMessages;
}

// ---------------------------------------------------------------------

void Log::add(WSJCppColorModifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage) {
    Log::initGlobalVariables();
    Log::doLogRotateUpdateFilename();

    std::lock_guard<std::mutex> lock(*g_LOG_MUTEX);
    WSJCppColorModifier def(WSJCppColorCode::FG_DEFAULT);

    std::string sLogMessage = WSJCppCore::currentTime_logformat() + ", " + WSJCppCore::threadId()
         + " [" + sType + "] " + sTag + ": " + sMessage;
    std::cout << clr << sLogMessage << def << std::endl;

    g_LAST_LOG_MESSAGES->push_front(sLogMessage);
    while (g_LAST_LOG_MESSAGES->size() > 50) {
        g_LAST_LOG_MESSAGES->pop_back();
    }
    // TODO try create global variable
    std::ofstream logFile(Log::g_LOG_FILE, std::ios::app);
    if (!logFile) {
        std::cout << "Error Opening File" << std::endl;
        return;
    }

    logFile << sLogMessage << std::endl;
    logFile.close();
}

// ---------------------------------------------------------------------
// WJSCppParseConfig
// TODO rename WJSCppParseConfig

WJSCppParseConfig::WJSCppParseConfig(const std::string &sFilepathConf) {
    TAG = "WJSCppParseConfig";
    m_sFilepathConf = sFilepathConf;
}

// ---------------------------------------------------------------------

bool WJSCppParseConfig::load() {
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

        WSJCppCore::trim(sLine);
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
            WSJCppCore::trim(sParamName);
            WSJCppCore::trim(sParamValue);
            
            // std::cout << " [" << sParamName << "]  => [" << sParamValue << "]" << std::endl;
            
            if (m_mapConfigValues.count(sParamName)) {
                Log::warn(TAG, "Ignoring duplicate of option line(" + std::to_string(nLineNumber) + ") in config: " + m_sFilepathConf);
            } else {
                m_mapConfigValues.insert(std::pair<std::string,std::string>(sParamName, sParamValue));    
            }
        } else {
            Log::warn(TAG, "Ignoring invalid line(" + std::to_string(nLineNumber) + ") in config: " + m_sFilepathConf);
        }
        
    }
    return true;
}

// ---------------------------------------------------------------------

bool WJSCppParseConfig::has(const std::string &sParamName) {
    return m_mapConfigValues.count(sParamName);
}

// ---------------------------------------------------------------------

std::string WJSCppParseConfig::stringValue(const std::string &sParamName, const std::string &defaultValue) {
    std::string sResult = defaultValue;
    if (m_mapConfigValues.count(sParamName)) {
        sResult = m_mapConfigValues.at(sParamName);
    } else {
        Log::warn(TAG, sParamName + " - not found in " + m_sFilepathConf + "\n\t Will be used default value: " + defaultValue);
    }
    return sResult;
}

// ---------------------------------------------------------------------

int WJSCppParseConfig::intValue(const std::string &sParamName, int defaultValue) {
    int nResult = defaultValue;
    if (m_mapConfigValues.count(sParamName)) {
        std::string sParamValue = m_mapConfigValues.at(sParamName);
        std::istringstream isBuffer(sParamValue);
        isBuffer >> nResult;
    } else {
        Log::warn(TAG, sParamName + " - not found in " + m_sFilepathConf + "\n\t Will be used default value: " + std::to_string(defaultValue));
    }
    return nResult;
}

// ---------------------------------------------------------------------

bool WJSCppParseConfig::boolValue(const std::string &sParamName, bool defaultValue) {
    bool bResult = defaultValue;

    if (m_mapConfigValues.count(sParamName)) {
        std::string sParamValue = m_mapConfigValues.at(sParamName);
        std::transform(sParamValue.begin(), sParamValue.end(), sParamValue.begin(), ::tolower);
        bResult = (sParamValue == "yes");
    } else {
        Log::warn(TAG, sParamName + " - not found in " + m_sFilepathConf + "\n\t Will be used default value: " + (defaultValue ? "yes" : "no"));
    }
    return bResult;
}

// ---------------------------------------------------------------------

std::map<std::string,std::string> WJSCppParseConfig::getValues() {
    return m_mapConfigValues;
}
