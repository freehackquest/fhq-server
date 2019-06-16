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

// ---------------------------------------------------------------------

long Fallen::currentTime_milliseconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

long Fallen::currentTime_seconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

std::string Fallen::currentTime_logformat() {
    long nTimeStart = Fallen::currentTime_milliseconds();
    std::string sMilliseconds = std::to_string(int(nTimeStart % 1000));
    nTimeStart = nTimeStart / 1000;

    std::time_t tm_ = long(nTimeStart);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf) + "." + std::string( 3 - sMilliseconds.length(), '0').append(sMilliseconds);
}

// ---------------------------------------------------------------------

std::string Fallen::threadId() {
    std::thread::id this_id = std::this_thread::get_id();
    std::stringstream stream;
    stream << std::hex << this_id;
    return "0x" + std::string(stream.str());
}

// ---------------------------------------------------------------------

std::string Fallen::formatTimeForWeb(long nTimeInSec) {
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    // Like: Wed, 22 Jul 2009 19:15:56 GMT
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tstruct);
    return std::string(buf);
}

// ---------------------------------------------------------------------

std::string Fallen::currentTime_forFilename() {
    long nTimeStart = Fallen::currentTime_seconds();
    return Fallen::formatTimeForFilename(nTimeStart);
}

// ---------------------------------------------------------------------

std::string Fallen::formatTimeForFilename(long nTimeInSec) {
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
    return std::string(buf);
}

// ---------------------------------------------------------------------

std::string Fallen::formatTimeUTC(int nTimeInSec) {
    // datetime
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf);
}

// ---------------------------------------------------------------------

bool Fallen::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
    return false;
}

// ---------------------------------------------------------------------

bool Fallen::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
    return false;
}

// ---------------------------------------------------------------------

std::vector<std::string> Fallen::listOfDirs(const std::string &sDirname) {
    std::vector<std::string> vDirs;
    if (!Fallen::dirExists(sDirname)) {
        return vDirs;
    }
    DIR *dir = opendir(sDirname.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type == DT_DIR) {
            std::string sDir(entry->d_name);
            if (sDir != "." && sDir != "..") {
                vDirs.push_back(sDir);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return vDirs;
}

// ---------------------------------------------------------------------

std::vector<std::string> Fallen::listOfFiles(const std::string &sDirname) {
    std::vector<std::string> vFiles;
    if (!Fallen::dirExists(sDirname)) {
        return vFiles;
    }
    DIR *dir = opendir(sDirname.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type != DT_DIR) {
            std::string sDir(entry->d_name);
            if (sDir != "." && sDir != "..") {
                vFiles.push_back(sDir);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return vFiles;
}

// ---------------------------------------------------------------------

bool Fallen::makeDir(const std::string &sDirname) {
    struct stat st;
    int nStatus = mkdir(sDirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (nStatus == 0) {
        return true;
    }
    if (nStatus == EACCES) {
        std::cout << "FAILED create folder " << sDirname << std::endl;
        return false;
    }
    std::cout << "nStatus: " << nStatus << std::endl;
    return true;
}

// ---------------------------------------------------------------------

bool Fallen::writeFile(const std::string &sFilename, const std::string &sContent) {
    
    std::ofstream f(sFilename, std::ios::out);
    if (!f) {
        std::cout << "FAILED could not create file to wtite " << sFilename << std::endl;
        return false;
    }

    f << sContent << std::endl;
    f.close();
    return true;
}

// ---------------------------------------------------------------------

bool Fallen::writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize) {
    std::ofstream f(sFilename, std::ios::out | std::ios::binary);
    if (!f) {
        std::cout << "FAILED could not create file to wtite " << sFilename << std::endl;
        return false;
    }
    f.write(pBuffer, nBufferSize);
    f.close();
    return true;
}


// ---------------------------------------------------------------------

std::string& Fallen::ltrim(std::string& str, const std::string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

// ---------------------------------------------------------------------

std::string& Fallen::rtrim(std::string& str, const std::string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

// ---------------------------------------------------------------------

std::string& Fallen::trim(std::string& str, const std::string& chars) {
    return Fallen::ltrim(Fallen::rtrim(str, chars), chars);
}

// ---------------------------------------------------------------------

std::string& Fallen::to_lower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}


// ---------------------------------------------------------------------

void Fallen::initRandom() {
    std::srand(std::rand() + std::time(0));
}

// ---------------------------------------------------------------------

std::string Fallen::createUuid() {
    std::string sRet = "00000000-0000-0000-0000-000000000000";
    const std::string sAlphabet = "0123456789abcdef";
    // unsigned t = std::time(0);
    for (int i = 0; i < 36; i++) {
        if (i != 8 && i != 13 && i != 18 && i != 23) {
            sRet[i] = sAlphabet[std::rand() % sAlphabet.length()];
        }
    }
    // Fallen::initRandom();
    return sRet;
}

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
    long t = Fallen::currentTime_seconds();
    long nEverySeconds = 51000; // rotate log if started now or if time left more then 1 day
    if (g_LOG_START_TIME == 0 || t - g_LOG_START_TIME > nEverySeconds || bForce) {
        g_LOG_START_TIME = t;
        g_LOG_FILE = g_LOG_DIR + "/" + Log::g_PREFIX_LOG_FILE + "_" + Fallen::formatTimeForFilename(g_LOG_START_TIME) + ".log";
    }
}

// ---------------------------------------------------------------------

void Log::info(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier def(Color::FG_DEFAULT);
    Log::add(def, "INFO", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::err(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier red(Color::FG_RED);
    Log::add(red, "ERR", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::throw_err(const std::string &sTag, const std::string &sMessage) {
    Color::Modifier red(Color::FG_RED);
    Log::add(red, "ERR", sTag, sMessage);
    throw std::runtime_error(sMessage);
}

// ---------------------------------------------------------------------

void Log::warn(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier yellow(Color::FG_YELLOW);
    Log::add(yellow, "WARN",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::ok(const std::string &sTag, const std::string &sMessage) {
    Color::Modifier green(Color::FG_GREEN);
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

void Log::add(Color::Modifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage) {
    Log::initGlobalVariables();
    Log::doLogRotateUpdateFilename();

    std::lock_guard<std::mutex> lock(*g_LOG_MUTEX);
    Color::Modifier def(Color::FG_DEFAULT);

    std::string sLogMessage = Fallen::currentTime_logformat() + ", " + Fallen::threadId()
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

        Fallen::trim(sLine);
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
            Fallen::trim(sParamName);
            Fallen::trim(sParamValue);
            
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
