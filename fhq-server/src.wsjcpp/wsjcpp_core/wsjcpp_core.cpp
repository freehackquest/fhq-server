#include "wsjcpp_core.h"
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
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
#include <streambuf>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// ---------------------------------------------------------------------

bool WSJCppCore::init(
    int argc, char** argv, 
    const std::string &sApplicationName,
    const std::string &sApplicationVersion,
    const std::string &sApplicationAuthor,
    const std::string &sLibraryNameForExports
) {
    // init random
    std::srand(std::rand() + std::time(0));
    // WSJCppCore::initRandom();
    return true;
}

// ---------------------------------------------------------------------

std::string WSJCppCore::doNormalizePath(const std::string & sPath) {
    // split path by /
    std::vector<std::string> vNames;
    std::string s = "";
    int nStrLen = sPath.length();
    for (int i = 0; i < sPath.length(); i++) {
        if (sPath[i] == '/') {
            vNames.push_back(s);
            s = "";
            if (i == nStrLen-1) {
                vNames.push_back("");
            }
        } else {
            s += sPath[i];
        }
    }
    if (s != "") {
         vNames.push_back(s);
    }

    // fildered
    int nLen = vNames.size();
    std::vector<std::string> vNewNames;
    for (int i = 0; i < nLen; i++) {
        std::string sCurrent = vNames[i];
        if (sCurrent == "" && i == nLen-1) {
            vNewNames.push_back(sCurrent);
            continue;
        }

        if ((sCurrent == "" || sCurrent == ".") && i != 0) {
            continue;
        }

        if (sCurrent == ".." && vNewNames.size() > 0) {
            std::string sPrev = vNewNames[vNewNames.size()-1];
            if (sPrev == "") {
                vNewNames.pop_back();
                vNewNames.push_back(sCurrent);
            } else if (sPrev != "." && sPrev != "..") {
                vNewNames.pop_back();
            } else {
                vNewNames.push_back(sCurrent);
            }
        } else {
            vNewNames.push_back(sCurrent);
        }
    }
    std::string sRet = "";
    int nNewLen = vNewNames.size();
    int nLastNew = nNewLen-1;
    for (int i = 0; i < nNewLen; i++) {
        sRet += vNewNames[i];
        if (i != nLastNew) {
            sRet += "/";
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WSJCppCore::extractFilename(const std::string &sPath) {
    // split path by /
    std::vector<std::string> vNames;
    std::string s = "";
    int nStrLen = sPath.length();
    for (int i = 0; i < sPath.length(); i++) {
        if (sPath[i] == '/') {
            vNames.push_back(s);
            s = "";
            if (i == nStrLen-1) {
                vNames.push_back("");
            }
        } else {
            s += sPath[i];
        }
    }
    if (s != "") {
         vNames.push_back(s);
    }
    std::string sRet;
    if (vNames.size() > 0) {
        sRet = vNames[vNames.size()-1];
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WSJCppCore::getCurrentDirectory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        WSJCppLog::throw_err("getCurrentDirectory", "Could not get current directory");
    }
    return std::string(cwd) + "/";
}

// ---------------------------------------------------------------------

long WSJCppCore::currentTime_milliseconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

long WSJCppCore::currentTime_seconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

std::string WSJCppCore::currentTime_logformat() {
    long nTimeStart = WSJCppCore::currentTime_milliseconds();
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

std::string WSJCppCore::threadId() {
    std::thread::id this_id = std::this_thread::get_id();
    std::stringstream stream;
    stream << std::hex << this_id;
    return "0x" + std::string(stream.str());
}

// ---------------------------------------------------------------------

std::string WSJCppCore::formatTimeForWeb(long nTimeInSec) {
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

std::string WSJCppCore::currentTime_forFilename() {
    long nTimeStart = WSJCppCore::currentTime_seconds();
    return WSJCppCore::formatTimeForFilename(nTimeStart);
}

// ---------------------------------------------------------------------

std::string WSJCppCore::formatTimeForFilename(long nTimeInSec) {
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

std::string WSJCppCore::formatTimeUTC(int nTimeInSec) {
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

bool WSJCppCore::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
    return false;
}

// ---------------------------------------------------------------------

bool WSJCppCore::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
    return false;
}

// ---------------------------------------------------------------------

std::vector<std::string> WSJCppCore::listOfDirs(const std::string &sDirname) {
    std::vector<std::string> vDirs;
    if (!WSJCppCore::dirExists(sDirname)) {
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

std::vector<std::string> WSJCppCore::listOfFiles(const std::string &sDirname) {
    std::vector<std::string> vFiles;
    if (!WSJCppCore::dirExists(sDirname)) {
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

bool WSJCppCore::makeDir(const std::string &sDirname) {
    struct stat st;
    int nStatus = mkdir(sDirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (nStatus == 0) {
        return true;
    }
    if (nStatus == EACCES) {
        std::cout << "FAILED create folder " << sDirname << std::endl;
        return false;
    }
    // std::cout << "nStatus: " << nStatus << std::endl;
    return true;
}

// ---------------------------------------------------------------------

bool WSJCppCore::writeFile(const std::string &sFilename, const std::string &sContent) {
    
    // std::ofstream f(sFilename, std::ifstream::in);
    std::ofstream f(sFilename, std::ios::out);
    if (!f) {
        WSJCppLog::err("WSJCppCore", "Could not create file to write '" + sFilename + "'");
        return false;
    }
    f << sContent << std::endl;
    f.close();
    return true;
}

// ---------------------------------------------------------------------

bool WSJCppCore::readTextFile(const std::string &sFilename, std::string &sContent) {
    
    std::ifstream f(sFilename);
    if (!f) {
        std::cout << "FAILED could not open file to read " << sFilename << std::endl;
        return false;
    }

    sContent = std::string(
        (std::istreambuf_iterator<char>(f)),
        std::istreambuf_iterator<char>()
    );

    return true;
}

// ---------------------------------------------------------------------

bool WSJCppCore::writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize) {
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

bool WSJCppCore::removeFile(const std::string &sFilename) {
    return remove(sFilename.c_str()) == 0;
}

// ---------------------------------------------------------------------

std::string& WSJCppCore::ltrim(std::string& str, const std::string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

// ---------------------------------------------------------------------

std::string& WSJCppCore::rtrim(std::string& str, const std::string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

// ---------------------------------------------------------------------

std::string& WSJCppCore::trim(std::string& str, const std::string& chars) {
    return WSJCppCore::ltrim(WSJCppCore::rtrim(str, chars), chars);
}

// ---------------------------------------------------------------------

std::string& WSJCppCore::to_lower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// ---------------------------------------------------------------------
// will worked only with latin

std::string WSJCppCore::toUpper(const std::string& str) {
    std::string sRet = str;
    std::transform(sRet.begin(), sRet.end(), sRet.begin(), ::toupper);
    return sRet;
}

// ---------------------------------------------------------------------

void WSJCppCore::initRandom() {
    std::srand(std::rand() + std::time(0));
}

// ---------------------------------------------------------------------

std::string WSJCppCore::createUuid() {
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

unsigned long WSJCppCore::convertVoidToULong(void *p) {
    unsigned long ret = *(unsigned long *)p;
    return ret;
}

// ---------------------------------------------------------------------

std::string WSJCppCore::getPointerAsHex(void *p) {
    std::uintptr_t i = reinterpret_cast<std::uintptr_t>(p);
    std::stringstream stream;
    stream << std::hex << i;
    return "0x" + std::string(stream.str());
}

// ---------------------------------------------------------------------

std::string WSJCppCore::extractURLProtocol(const std::string& sValue) {
    std::string sRet = "";
    int nPosProtocol = sValue.find("://");
    if (nPosProtocol == std::string::npos) {
        return sRet;
    }
    sRet = sValue.substr(0, nPosProtocol);
    return sRet;
}

// ---------------------------------------------------------------------
// WSJCppLog

// Last log messages
std::deque<std::string> * WSJCppLog::g_WSJCPP_LOG_LAST_MESSAGES = nullptr;
std::mutex * WSJCppLog::g_WSJCPP_LOG_MUTEX = nullptr;
std::string WSJCppLog::g_WSJCPP_LOG_DIR = "./";
std::string WSJCppLog::g_WSJCPP_LOG_FILE = "";
std::string WSJCppLog::g_WSJCPP_LOG_PREFIX_FILE = "";
long WSJCppLog::g_WSJCPP_LOG_START_TIME = 0;

// ---------------------------------------------------------------------

void WSJCppLog::doLogRotateUpdateFilename(bool bForce) {
    long t = WSJCppCore::currentTime_seconds();
    long nEverySeconds = 51000; // rotate log if started now or if time left more then 1 day
    if (g_WSJCPP_LOG_START_TIME == 0 || t - g_WSJCPP_LOG_START_TIME > nEverySeconds || bForce) {
        g_WSJCPP_LOG_START_TIME = t;
        g_WSJCPP_LOG_FILE = g_WSJCPP_LOG_DIR + "/"
            + WSJCppLog::g_WSJCPP_LOG_PREFIX_FILE + "_"
            + WSJCppCore::formatTimeForFilename(g_WSJCPP_LOG_START_TIME) + ".log";
    }
}

// ---------------------------------------------------------------------

void WSJCppLog::info(const std::string & sTag, const std::string &sMessage) {
    WSJCppColorModifier def(WSJCppColorCode::FG_DEFAULT);
    WSJCppLog::add(def, "INFO", sTag, sMessage);
}

// ---------------------------------------------------------------------

void WSJCppLog::err(const std::string & sTag, const std::string &sMessage) {
    WSJCppColorModifier red(WSJCppColorCode::FG_RED);
    WSJCppLog::add(red, "ERR", sTag, sMessage);
}

// ---------------------------------------------------------------------

void WSJCppLog::throw_err(const std::string &sTag, const std::string &sMessage) {
    WSJCppColorModifier red(WSJCppColorCode::FG_RED);
    WSJCppLog::add(red, "ERR", sTag, sMessage);
    throw std::runtime_error(sMessage);
}

// ---------------------------------------------------------------------

void WSJCppLog::warn(const std::string & sTag, const std::string &sMessage) {
    WSJCppColorModifier yellow(WSJCppColorCode::FG_YELLOW);
    WSJCppLog::add(yellow, "WARN",sTag, sMessage);
}

// ---------------------------------------------------------------------

void WSJCppLog::ok(const std::string &sTag, const std::string &sMessage) {
    WSJCppColorModifier green(WSJCppColorCode::FG_GREEN);
    WSJCppLog::add(green, "OK", sTag, sMessage);
}

// ---------------------------------------------------------------------

std::vector<std::string> WSJCppLog::getLastLogMessages() {
    WSJCppLog::initGlobalVariables();
    std::lock_guard<std::mutex> lock(*WSJCppLog::g_WSJCPP_LOG_MUTEX);
    std::vector<std::string> vRet;
    for (int i = 0; i < g_WSJCPP_LOG_LAST_MESSAGES->size(); i++) {
        vRet.push_back(g_WSJCPP_LOG_LAST_MESSAGES->at(i));
    }
    return vRet;
}

// ---------------------------------------------------------------------

void WSJCppLog::setLogDirectory(const std::string &sDirectoryPath) {
    WSJCppLog::g_WSJCPP_LOG_DIR = sDirectoryPath;
    WSJCppLog::doLogRotateUpdateFilename(true);
}

// ---------------------------------------------------------------------

void WSJCppLog::setPrefixLogFile(const std::string &sPrefixLogFile) {
    WSJCppLog::g_WSJCPP_LOG_PREFIX_FILE = sPrefixLogFile;
    WSJCppLog::doLogRotateUpdateFilename(true);
}

// ---------------------------------------------------------------------

void WSJCppLog::initGlobalVariables() {
    // create deque if not created
    if (WSJCppLog::g_WSJCPP_LOG_LAST_MESSAGES == nullptr) {
        WSJCppLog::g_WSJCPP_LOG_LAST_MESSAGES = new std::deque<std::string>();
        // std::cout << WSJCppCore::currentTime_logformat() + ", " + WSJCppCore::threadId() + " Init last messages deque\r\n";
    }
    // create mutex if not created
    if (WSJCppLog::g_WSJCPP_LOG_MUTEX == nullptr) {
        WSJCppLog::g_WSJCPP_LOG_MUTEX = new std::mutex();
        // std::cout << WSJCppCore::currentTime_logformat() + ", " + WSJCppCore::threadId() + " Init mutex for log\r\n";
    }
}

// ---------------------------------------------------------------------

void WSJCppLog::add(WSJCppColorModifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage) {
    WSJCppLog::initGlobalVariables();
    WSJCppLog::doLogRotateUpdateFilename();

    std::lock_guard<std::mutex> lock(*WSJCppLog::g_WSJCPP_LOG_MUTEX);
    WSJCppColorModifier def(WSJCppColorCode::FG_DEFAULT);

    std::string sLogMessage = WSJCppCore::currentTime_logformat() + ", " + WSJCppCore::threadId()
         + " [" + sType + "] " + sTag + ": " + sMessage;
    std::cout << clr << sLogMessage << def << std::endl;

    g_WSJCPP_LOG_LAST_MESSAGES->push_front(sLogMessage);
    while (g_WSJCPP_LOG_LAST_MESSAGES->size() > 50) {
        g_WSJCPP_LOG_LAST_MESSAGES->pop_back();
    }
    // TODO try create global variable
    std::ofstream logFile(WSJCppLog::g_WSJCPP_LOG_FILE, std::ios::app);
    if (!logFile) {
        std::cout << "Error Opening File" << std::endl;
        return;
    }

    logFile << sLogMessage << std::endl;
    logFile.close();
}


