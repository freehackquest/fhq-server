#include "wsjcpp_core.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
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
// #include <sys/socket.h>
#include <arpa/inet.h>
#include <random>
#include <iomanip>

// ---------------------------------------------------------------------
// WsjcppFilePermissions

WsjcppFilePermissions::WsjcppFilePermissions() {
    // default permissions
    m_bOwnerReadFlag = true;
    m_bOwnerWriteFlag = true;
    m_bOwnerExecuteFlag = false;
    m_bGroupReadFlag = false;
    m_bGroupWriteFlag = false;
    m_bGroupExecuteFlag = false;
    m_bOtherReadFlag = true;
    m_bOtherWriteFlag = false;
    m_bOtherExecuteFlag = false;
}

WsjcppFilePermissions::WsjcppFilePermissions(
    bool bOwnerReadFlag, bool bOwnerWriteFlag, bool bOwnerExecuteFlag,
    bool bGroupReadFlag, bool bGroupWriteFlag, bool bGroupExecuteFlag,
    bool bOtherReadFlag, bool bOtherWriteFlag, bool bOtherExecuteFlag
) {
    m_bOwnerReadFlag = bOwnerReadFlag;
    m_bOwnerWriteFlag = bOwnerWriteFlag;
    m_bOwnerExecuteFlag = bOwnerExecuteFlag;
    m_bGroupReadFlag = bGroupReadFlag;
    m_bGroupWriteFlag = bGroupWriteFlag;
    m_bGroupExecuteFlag = bGroupExecuteFlag;
    m_bOtherReadFlag = bOtherReadFlag;
    m_bOtherWriteFlag = bOtherWriteFlag;
    m_bOtherExecuteFlag = bOtherExecuteFlag;
}

WsjcppFilePermissions::WsjcppFilePermissions(uint16_t nFilePermission) {
    
    // owner
    m_bOwnerReadFlag = nFilePermission & 0x0400;
    m_bOwnerWriteFlag = nFilePermission & 0x0200;
    m_bOwnerExecuteFlag = nFilePermission & 0x0100;

    // group
    m_bGroupReadFlag = nFilePermission & 0x0040;
    m_bGroupWriteFlag = nFilePermission & 0x0020;
    m_bGroupExecuteFlag = nFilePermission & 0x0010;

    // for other
    m_bOtherReadFlag = nFilePermission & 0x0004;
    m_bOtherWriteFlag = nFilePermission & 0x0002;
    m_bOtherExecuteFlag = nFilePermission & 0x0001;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setOwnerReadFlag(bool bOwnerReadFlag) {
    m_bOwnerReadFlag = bOwnerReadFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getOwnerReadFlag() const {
    return m_bOwnerReadFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setOwnerWriteFlag(bool bOwnerWriteFlag) {
    m_bOwnerWriteFlag = bOwnerWriteFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getOwnerWriteFlag() const {
    return m_bOwnerWriteFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setOwnerExecuteFlag(bool bOwnerExecuteFlag) {
    m_bOwnerExecuteFlag = bOwnerExecuteFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getOwnerExecuteFlag() const {
    return m_bOwnerExecuteFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setOwnerFlags(bool bOwnerReadFlag, bool bOwnerWriteFlag, bool bOwnerExecuteFlag) {
    m_bOwnerReadFlag = bOwnerReadFlag;
    m_bOwnerWriteFlag = bOwnerWriteFlag;
    m_bOwnerExecuteFlag = bOwnerExecuteFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setGroupReadFlag(bool bGroupReadFlag) {
    m_bGroupReadFlag = bGroupReadFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getGroupReadFlag() const {
    return m_bGroupReadFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setGroupWriteFlag(bool bGroupWriteFlag) {
    m_bGroupWriteFlag = bGroupWriteFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getGroupWriteFlag() const {
    return m_bGroupWriteFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setGroupExecuteFlag(bool bGroupExecuteFlag) {
    m_bGroupExecuteFlag = bGroupExecuteFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getGroupExecuteFlag() const {
    return m_bGroupExecuteFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setGroupFlags(bool bGroupReadFlag, bool bGroupWriteFlag, bool bGroupExecuteFlag) {
    m_bGroupReadFlag = bGroupReadFlag;
    m_bGroupWriteFlag = bGroupWriteFlag;
    m_bGroupExecuteFlag = bGroupExecuteFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setOtherReadFlag(bool bOtherReadFlag) {
    m_bOtherReadFlag = bOtherReadFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getOtherReadFlag() const {
    return m_bOtherReadFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setOtherWriteFlag(bool bOtherWriteFlag) {
    m_bOtherWriteFlag = bOtherWriteFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getOtherWriteFlag() const {
    return m_bOtherWriteFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setOtherExecuteFlag(bool bOtherExecuteFlag) {
    m_bOtherExecuteFlag = bOtherExecuteFlag;
}

// ---------------------------------------------------------------------

bool WsjcppFilePermissions::getOtherExecuteFlag() const {
    return m_bOtherExecuteFlag;
}

// ---------------------------------------------------------------------

void WsjcppFilePermissions::setOtherFlags(bool bOtherReadFlag, bool bOtherWriteFlag, bool bOtherExecuteFlag) {
    m_bOtherReadFlag = bOtherReadFlag;
    m_bOtherWriteFlag = bOtherWriteFlag;
    m_bOtherExecuteFlag = bOtherExecuteFlag;
}

// ---------------------------------------------------------------------

std::string WsjcppFilePermissions::toString() const {
    std::string sRet = "-";

    // owner
    sRet += m_bOwnerReadFlag ? "r" : "-";
    sRet += m_bOwnerWriteFlag ? "w" : "-";
    sRet += m_bOwnerExecuteFlag ? "x" : "-";

    // group
    sRet += m_bGroupReadFlag ? "r" : "-";
    sRet += m_bGroupWriteFlag ? "w" : "-";
    sRet += m_bGroupExecuteFlag ? "x" : "-";

    // for other
    sRet += m_bOtherReadFlag ? "r" : "-";
    sRet += m_bOtherWriteFlag ? "w" : "-";
    sRet += m_bOtherExecuteFlag ? "x" : "-";

    return sRet;
}

// ---------------------------------------------------------------------

uint16_t WsjcppFilePermissions::toUInt16() const {
    uint16_t nRet = 0x0;
    // owner
    nRet |= m_bOwnerReadFlag ? 0x0400 : 0x0;
    nRet |= m_bOwnerWriteFlag ? 0x0200 : 0x0;
    nRet |= m_bOwnerExecuteFlag ? 0x0100 : 0x0;

    // group
    nRet += m_bGroupReadFlag ? 0x0040 : 0x0;
    nRet += m_bGroupWriteFlag ? 0x0020 : 0x0;
    nRet += m_bGroupExecuteFlag ? 0x0010 : 0x0;

    // for other
    nRet += m_bOtherReadFlag ? 0x0004 : 0x0;
    nRet += m_bOtherWriteFlag ? 0x0002 : 0x0;
    nRet += m_bOtherExecuteFlag ? 0x0001 : 0x0;
    return nRet;
}


// ---------------------------------------------------------------------
// WsjcppCore

bool WsjcppCore::init(
    int argc, char** argv, 
    const std::string &sApplicationName,
    const std::string &sApplicationVersion,
    const std::string &sApplicationAuthor,
    const std::string &sLibraryNameForExports
) {
    // init random
    // std::srand(std::time(0));
    WsjcppCore::initRandom();
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::doNormalizePath(const std::string & sPath) {
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

std::string WsjcppCore::extractFilename(const std::string &sPath) {
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

std::string WsjcppCore::getCurrentDirectory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        WsjcppLog::throw_err("getCurrentDirectory", "Could not get current directory");
    }
    return std::string(cwd) + "/";
}

// ---------------------------------------------------------------------

long WsjcppCore::getCurrentTimeInMilliseconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

long WsjcppCore::getCurrentTimeInSeconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::getCurrentTimeForFilename() {
    long nTimeStart = WsjcppCore::getCurrentTimeInSeconds();
    return WsjcppCore::formatTimeForFilename(nTimeStart);
}

// ---------------------------------------------------------------------

std::string WsjcppCore::getCurrentTimeForLogFormat() {
    long nTimeStart = WsjcppCore::getCurrentTimeInMilliseconds();
    std::string sMilliseconds = std::to_string(int(nTimeStart % 1000));
    nTimeStart = nTimeStart / 1000;

    std::time_t tm_ = long(nTimeStart);
    struct tm tstruct;
    gmtime_r(&tm_, &tstruct);

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf) + "." + std::string( 3 - sMilliseconds.length(), '0').append(sMilliseconds);
}

// ---------------------------------------------------------------------

std::string WsjcppCore::getThreadId() {

    static_assert(sizeof(std::thread::id)==sizeof(uint64_t),"this function only works if size of thead::id is equal to the size of uint_64");
    std::thread::id this_id = std::this_thread::get_id();
    uint64_t val = *((uint64_t*) &this_id);
    std::stringstream stream;
    stream << "0x" << std::setw(16) << std::setfill('0') << std::hex << val;
    return std::string(stream.str());
}

// ---------------------------------------------------------------------

std::string WsjcppCore::formatTimeForWeb(long nTimeInSec) {
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct;
    gmtime_r(&tm_, &tstruct);
    
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    // Like: Wed, 22 Jul 2009 19:15:56 GMT
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tstruct);
    return std::string(buf);
}



// ---------------------------------------------------------------------

std::string WsjcppCore::formatTimeForFilename(long nTimeInSec) {
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct;
    gmtime_r(&tm_, &tstruct);

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
    return std::string(buf);
}

// ---------------------------------------------------------------------

std::string WsjcppCore::formatTimeUTC(int nTimeInSec) {
    // datetime
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct;
    gmtime_r(&tm_, &tstruct);

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf);
}

// ---------------------------------------------------------------------

bool WsjcppCore::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppCore::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
    return false;
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppCore::listOfDirs(const std::string &sDirname) {
    WsjcppLog::warn("listOfDirs", "Deprecated. Use a WsjcppCore::getListOfDirs");
    return WsjcppCore::getListOfDirs(sDirname);
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppCore::getListOfDirs(const std::string &sDirname) {
    std::vector<std::string> vDirs;
    if (!WsjcppCore::dirExists(sDirname)) {
        return vDirs;
    }
    DIR *dir = opendir(sDirname.c_str());
    if (dir != NULL) {
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
    }
    std::sort(vDirs.begin(), vDirs.end());
    return vDirs;
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppCore::listOfFiles(const std::string &sDirname) {
    WsjcppLog::warn("listOfFiles", "Deprecated. Use a WsjcppCore::getListOfFiles");
    return WsjcppCore::getListOfFiles(sDirname);
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppCore::getListOfFiles(const std::string &sDirname) {
    std::vector<std::string> vFiles;
    if (!WsjcppCore::dirExists(sDirname)) {
        return vFiles;
    }
    DIR *dir = opendir(sDirname.c_str());
    if (dir != NULL) {
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
    }
    return vFiles;
}

// ---------------------------------------------------------------------

bool WsjcppCore::makeDir(const std::string &sDirname) {
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

bool WsjcppCore::writeFile(const std::string &sFilename, const std::string &sContent) {
    
    // std::ofstream f(sFilename, std::ifstream::in);
    std::ofstream f(sFilename, std::ios::out);
    if (!f) {
        WsjcppLog::err("WsjcppCore", "Could not create file to write '" + sFilename + "'");
        return false;
    }
    f << sContent << std::endl;
    f.close();
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::readTextFile(const std::string &sFilename, std::string &sContent) {
    
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

bool WsjcppCore::readFileToBuffer(const std::string &sFilename, char *pBuffer[], int &nBufferSize) {
    std::ifstream f(sFilename, std::ifstream::binary);
    if (!f) {
        return false;
    }

    // get length of file:
    f.seekg (0, f.end);
    nBufferSize = f.tellg();
    f.seekg (0, f.beg);

    *pBuffer = new char [nBufferSize];

    // read data as a block:
    f.read (*pBuffer, nBufferSize);
    if (!f) {
        WsjcppLog::err("WsjcppCore::readFileToBuffer", "Only " + std::to_string(f.gcount()) + " could be read");
        delete[] pBuffer;
        f.close();
        return false;
    }
    f.close();
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize) {
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

bool WsjcppCore::removeFile(const std::string &sFilename) {
    return remove(sFilename.c_str()) == 0;
}

// ---------------------------------------------------------------------

bool WsjcppCore::copyFile(const std::string &sSourceFilename, const std::string &sTargetFilename) {
    if (!WsjcppCore::fileExists(sSourceFilename)) {
        WsjcppLog::err("copyFile", "File '" + sSourceFilename + "' did not exists");
        return false;
    }

    if (WsjcppCore::fileExists(sTargetFilename)) {
        WsjcppLog::err("copyFile", "File '" + sTargetFilename + "' already exists");
        return false;
    }

    std::ifstream src(sSourceFilename, std::ios::binary);
    if (!src.is_open()) {
        WsjcppLog::err("copyFile", "Could not open file '" + sSourceFilename + "' for read");
        return false;
    }

    std::ofstream dst(sTargetFilename, std::ios::binary);
    if (!dst.is_open()) {
        WsjcppLog::err("copyFile", "Could not open file '" + sTargetFilename + "' for write");
        return false;
    }

    dst << src.rdbuf();
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::createEmptyFile(const std::string &sFilename) {
    if (WsjcppCore::fileExists(sFilename)) {
        return false;
    }
    std::ofstream f(sFilename, std::ios::out | std::ios::binary);
    if (!f) {
        std::cout << "FAILED could not create file to wtite " << sFilename << std::endl;
        return false;
    }
    f.close();
    return true;
}

// ---------------------------------------------------------------------

std::string& WsjcppCore::ltrim(std::string& str, const std::string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

// ---------------------------------------------------------------------

std::string& WsjcppCore::rtrim(std::string& str, const std::string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

// ---------------------------------------------------------------------

std::string& WsjcppCore::trim(std::string& str, const std::string& chars) {
    return WsjcppCore::ltrim(WsjcppCore::rtrim(str, chars), chars);
}

// ---------------------------------------------------------------------
// will worked only with latin

std::string WsjcppCore::toLower(const std::string& str) {
    std::string sRet = str;
    std::transform(sRet.begin(), sRet.end(), sRet.begin(), ::tolower);
    return sRet;
}

// ---------------------------------------------------------------------
// will worked only with latin

std::string WsjcppCore::toUpper(const std::string& str) {
    std::string sRet = str;
    std::transform(sRet.begin(), sRet.end(), sRet.begin(), ::toupper);
    return sRet;
}

// ---------------------------------------------------------------------

void WsjcppCore::replaceAll(std::string& str, const std::string& sFrom, const std::string& sTo) {
    if (sFrom.empty()) {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(sFrom, start_pos)) != std::string::npos) {
        str.replace(start_pos, sFrom.length(), sTo);
        start_pos += sTo.length(); // In case 'to' contains 'sFrom', like replacing 'x' with 'yx'
    }
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppCore::split(const std::string& sWhat, const std::string& sDelim) {
    std::vector<std::string> vRet;
    int nPos = 0;
    int nLen = sWhat.length();
    int nDelimLen = sDelim.length();
    while (nPos < nLen) {
        std::size_t nFoundPos = sWhat.find(sDelim, nPos);
        if (nFoundPos != std::string::npos) {
            std::string sToken = sWhat.substr(nPos, nFoundPos - nPos);
            vRet.push_back(sToken);
            nPos = nFoundPos + nDelimLen;
            if (nFoundPos + nDelimLen == nLen) { // last delimiter
                vRet.push_back("");
            }
        } else {
            std::string sToken = sWhat.substr(nPos, nLen - nPos);
            vRet.push_back(sToken);
            break;
        }
    }
    return vRet;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::join(const std::vector<std::string> &vWhat, const std::string& sDelim) {
    std::string sRet;
    for (int i = 0; i < vWhat.size(); i++) {
        if (i != 0) {
            sRet += sDelim;
        }
        sRet += vWhat[i];
    }
    return sRet;
}

// ---------------------------------------------------------------------

void WsjcppCore::initRandom() {
    std::srand(std::time(0));
}

// ---------------------------------------------------------------------

std::string WsjcppCore::createUuid() {
    std::string sRet = "00000000-0000-0000-0000-000000000000";
    const std::string sAlphabet = "0123456789abcdef";
    for (int i = 0; i < 36; i++) {
        if (i != 8 && i != 13 && i != 18 && i != 23) {
            sRet[i] = sAlphabet[rand() % sAlphabet.length()];
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::uint2hexString(unsigned int n) {
    std::string sRet;
    for (int i = 0; i < 8; i++) {
        sRet += "0123456789abcdef"[n % 16];
        n >>= 4;
    }
    return std::string(sRet.rbegin(), sRet.rend());
}

// ---------------------------------------------------------------------

unsigned long WsjcppCore::convertVoidToULong(void *p) {
    std::uintptr_t ret = reinterpret_cast<std::uintptr_t>(p);
    return (unsigned long)ret;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::getPointerAsHex(void *p) {
    std::uintptr_t i = reinterpret_cast<std::uintptr_t>(p);
    std::stringstream stream;
    stream << std::hex << i;
    return "0x" + std::string(stream.str());
}

// ---------------------------------------------------------------------

std::string WsjcppCore::extractURLProtocol(const std::string& sValue) {
    std::string sRet = "";
    int nPosProtocol = sValue.find("://");
    if (nPosProtocol == std::string::npos) {
        return sRet;
    }
    sRet = sValue.substr(0, nPosProtocol);
    return sRet;
}

// ---------------------------------------------------------------------

bool WsjcppCore::getEnv(const std::string& sName, std::string& sValue) {
    if (const char* env_p = std::getenv(sName.c_str())) {
        sValue = std::string(env_p);
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::encodeUriComponent(const std::string& sValue) {
    std::stringstream ssRet;
    for (int i = 0; i < sValue.length(); i++) {
        char c = sValue[i];
        if (
            c == '-' || c == '_' || c == '.' || c == '!'
            || c == '~' || c == '*' || c == '\'' 
            || c == '(' || c == ')' || (c >= '0' && c <= '9') 
            || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        ) {
            ssRet << c;
        } else {
            ssRet << '%' << std::uppercase << std::hex << (int)c;
        }
    }
    return ssRet.str();
}

// ---------------------------------------------------------------------

std::string WsjcppCore::decodeUriComponent(const std::string& sValue) {
    std::string sRet = "";
    std::string sHex = "";
    int nLen = sValue.length();
    for (int i = 0; i < sValue.length(); i++) {
        char c = sValue[i];
        if (c == '%') {
            if (i+2 >= nLen) {
                WsjcppLog::throw_err("WsjcppCore::decodeUriElement", "Wrong format of string");
            }
            sHex = "0x";
            sHex += sValue[i+1];
            sHex += sValue[i+2];
            i = i + 2;
            char c1 = std::stoul(sHex, nullptr, 16);
            sRet += c1;
        } else {
            sRet += c;
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::getHumanSizeBytes(long nBytes) {
    if (nBytes == 0) {
        return "0B";
    }
    std::string arrPrefix[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    long n0 = nBytes;
    long n1 = 0;
    for (int i = 0; i < 6; i++) {
        if (n0 >= 1 && n0 < 1000) {
            return std::to_string(n0) + arrPrefix[i];
        }        
        n0 = nBytes / 1000;
        n1 = nBytes - n0 * 1000;
        n0 += n1 >= 500 ? 1 : 0;

        nBytes = nBytes / 1000;
        if (n0 == 0 && n1 == 0) {
            return "fuck"; 
        }
    }
    return std::to_string(nBytes) + "PB";
}

// ---------------------------------------------------------------------

bool WsjcppCore::recoursiveCopyFiles(const std::string& sSourceDir, const std::string& sTargetDir) {
    if (!WsjcppCore::dirExists(sSourceDir)) {
        WsjcppLog::err("recoursiveCopyFiles", "Source Dir '" + sSourceDir + "' did not exists");
        return false;
    }

    if (!WsjcppCore::dirExists(sTargetDir)) {
        if (!WsjcppCore::makeDir(sTargetDir)) {
            WsjcppLog::err("recoursiveCopyFiles", "Could not create target dir '" + sTargetDir + "'");
            return false;
        }
    }

    std::vector<std::string> vFiles = WsjcppCore::getListOfFiles(sSourceDir);
    for (int i = 0; i < vFiles.size(); i++) {
        std::string sSourceFile = sSourceDir + "/" + vFiles[i];
        std::string sTargetFile = sTargetDir + "/" + vFiles[i];
        if (!WsjcppCore::copyFile(sSourceFile, sTargetFile)) {
            return false;
        }
    }

    std::vector<std::string> vDirs = WsjcppCore::getListOfDirs(sSourceDir);
    for (int i = 0; i < vDirs.size(); i++) {
        std::string sSourceDir2 = sSourceDir + "/" + vDirs[i];
        std::string sTargetDir2 = sTargetDir + "/" + vDirs[i];
        if (!WsjcppCore::dirExists(sTargetDir2)) {
            if (!WsjcppCore::makeDir(sTargetDir2)) {
                WsjcppLog::err("recoursiveCopyFiles", "Could not create target subdir '" + sTargetDir2 + "'");
                return false;
            }
        }

        if (!WsjcppCore::recoursiveCopyFiles(sSourceDir2, sTargetDir2)) {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::recoursiveRemoveDir(const std::string& sDir) {
    if (!WsjcppCore::dirExists(sDir)) {
        WsjcppLog::err("recoursiveCopyFiles", "Dir '" + sDir + "' did not exists");
        return false;
    }

    std::vector<std::string> vFiles = WsjcppCore::getListOfFiles(sDir);
    for (int i = 0; i < vFiles.size(); i++) {
        std::string sFile = sDir + "/" + vFiles[i];
        if (!WsjcppCore::removeFile(sFile)) {
            return false;
        }
    }

    std::vector<std::string> vDirs = WsjcppCore::getListOfDirs(sDir);
    for (int i = 0; i < vDirs.size(); i++) {
        std::string sDir2 = sDir + "/" + vDirs[i];
        if (!WsjcppCore::recoursiveRemoveDir(sDir2)) {
            return false;
        }
    }

    if (!WsjcppCore::removeFile(sDir)) {
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::setFilePermissions(const std::string& sFilePath, const WsjcppFilePermissions &filePermissions, std::string& sError) {

    mode_t m;

    // owner
    m |= filePermissions.getOwnerReadFlag() ? S_IRUSR : 0x0;
    m |= filePermissions.getOwnerWriteFlag() ? S_IWUSR : 0x0;
    m |= filePermissions.getOwnerExecuteFlag() ? S_IXUSR : 0x0;

    // group
    m |= filePermissions.getGroupReadFlag() ? S_IRGRP : 0x0;
    m |= filePermissions.getGroupWriteFlag() ? S_IWGRP : 0x0;
    m |= filePermissions.getGroupExecuteFlag() ? S_IXGRP : 0x0;

    // for other
    m |= filePermissions.getOtherReadFlag() ? S_IROTH : 0x0;
    m |= filePermissions.getOtherWriteFlag() ? S_IWOTH : 0x0;
    m |= filePermissions.getOtherExecuteFlag() ? S_IXOTH : 0x0;

    if (chmod(sFilePath.c_str(), m) != 0) {
        sError = "Could not change permissions for: '" + sFilePath + "'";
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::getFilePermissions(const std::string& sFilePath, WsjcppFilePermissions &filePermissions, std::string& sError) {
    if (!WsjcppCore::fileExists(sFilePath)) {
        sError = "File '" + sFilePath + "' - not found";
        return false;
    }

    struct stat fileStat;
    if (stat(sFilePath.c_str(), &fileStat) < 0) {
        sError = "Could not get info about file '" + sFilePath + "'.";
        return false;
    }

    mode_t m = fileStat.st_mode;

    // S_ISDIR(fileStat.st_mode)) ? "d" : "-"

    // owner
    filePermissions.setOwnerReadFlag(m & S_IRUSR);
    filePermissions.setOwnerWriteFlag(m & S_IWUSR);
    filePermissions.setOwnerExecuteFlag(m & S_IXUSR);

    
    // group
    filePermissions.setGroupReadFlag(m & S_IRGRP);
    filePermissions.setGroupWriteFlag(m & S_IWGRP);
    filePermissions.setGroupExecuteFlag(m & S_IXGRP);

    // for other
    filePermissions.setOtherReadFlag(m & S_IROTH);
    filePermissions.setOtherWriteFlag(m & S_IWOTH);
    filePermissions.setOtherExecuteFlag(m & S_IXOTH);

    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::doPadLeft(const std::string& sIn, char cWhat, int nLength) {
    std::string sRet;
    int nPadLen = nLength - sIn.length();
    for (int i = 0; i < nPadLen; i++) {
        sRet += cWhat;
    }
    return sRet + sIn; 
}

// ---------------------------------------------------------------------

std::string WsjcppCore::doPadRight(const std::string& sIn, char cWhat, int nLength) {
    std::string sRet;
    int nPadLen = nLength - sIn.length();
    for (int i = 0; i < nPadLen; i++) {
        sRet += cWhat;
    }
    return sIn + sRet;
}

// ---------------------------------------------------------------------
// WsjcppLog

WsjcppLogGlobalConf::WsjcppLogGlobalConf() {
    // 
    logDir = "./";
    logPrefixFile = "";
    logFile = "";
    enableLogFile = true;
    logStartTime = 0;
    logRotationPeriodInSeconds = 51000;
}

// ---------------------------------------------------------------------

void WsjcppLogGlobalConf::doLogRotateUpdateFilename(bool bForce) {
    long t = WsjcppCore::getCurrentTimeInSeconds();
    long nEverySeconds = logRotationPeriodInSeconds; // rotate log if started now or if time left more then 1 day
    if (logStartTime == 0 || t - logStartTime > nEverySeconds || bForce) {
        logStartTime = t;
        logFile = logDir + "/"
            + logPrefixFile + "_"
            + WsjcppCore::formatTimeForFilename(logStartTime) + ".log";
    }
}

WsjcppLogGlobalConf WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF;

// ---------------------------------------------------------------------

void WsjcppLog::info(const std::string & sTag, const std::string &sMessage) {
    WsjcppColorModifier def(WsjcppColorCode::FG_DEFAULT);
    WsjcppLog::add(def, "INFO", sTag, sMessage);
}

// ---------------------------------------------------------------------

void WsjcppLog::err(const std::string & sTag, const std::string &sMessage) {
    WsjcppColorModifier red(WsjcppColorCode::FG_RED);
    WsjcppLog::add(red, "ERR", sTag, sMessage);
}

// ---------------------------------------------------------------------

void WsjcppLog::throw_err(const std::string &sTag, const std::string &sMessage) {
    WsjcppColorModifier red(WsjcppColorCode::FG_RED);
    WsjcppLog::add(red, "ERR", sTag, sMessage);
    throw std::runtime_error(sMessage);
}

// ---------------------------------------------------------------------

void WsjcppLog::warn(const std::string & sTag, const std::string &sMessage) {
    WsjcppColorModifier yellow(WsjcppColorCode::FG_YELLOW);
    WsjcppLog::add(yellow, "WARN",sTag, sMessage);
}

// ---------------------------------------------------------------------

void WsjcppLog::ok(const std::string &sTag, const std::string &sMessage) {
    WsjcppColorModifier green(WsjcppColorCode::FG_GREEN);
    WsjcppLog::add(green, "OK", sTag, sMessage);
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppLog::getLastLogMessages() {
    std::lock_guard<std::mutex> lock(WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logMutex);
    std::vector<std::string> vRet;
    for (int i = 0; i < WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logLastMessages.size(); i++) {
        vRet.push_back(WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logLastMessages[i]);
    }
    return vRet;
}

// ---------------------------------------------------------------------

void WsjcppLog::setLogDirectory(const std::string &sDirectoryPath) {
    WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logDir = sDirectoryPath;
    if (!WsjcppCore::dirExists(sDirectoryPath)) {
        if (!WsjcppCore::makeDir(sDirectoryPath)) {
            WsjcppLog::err("setLogDirectory", "Could not create log directory '" + sDirectoryPath + "'");
        }
    }
    WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.doLogRotateUpdateFilename(true);
}

// ---------------------------------------------------------------------

void WsjcppLog::setPrefixLogFile(const std::string &sPrefixLogFile) {
    WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logPrefixFile = sPrefixLogFile;
    WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.doLogRotateUpdateFilename(true);
}

// ---------------------------------------------------------------------

void WsjcppLog::setEnableLogFile(bool bEnable) {
    WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.enableLogFile = bEnable;
}

// ---------------------------------------------------------------------

void WsjcppLog::setRotationPeriodInSec(long nRotationPeriodInSec) {
    WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logRotationPeriodInSeconds = nRotationPeriodInSec;
}

// ---------------------------------------------------------------------

void WsjcppLog::add(WsjcppColorModifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage) {
    WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.doLogRotateUpdateFilename();

    std::lock_guard<std::mutex> lock(WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logMutex);
    WsjcppColorModifier def(WsjcppColorCode::FG_DEFAULT);

    std::string sLogMessage = WsjcppCore::getCurrentTimeForLogFormat() + ", " + WsjcppCore::getThreadId()
         + " [" + sType + "] " + sTag + ": " + sMessage;
    std::cout << clr << sLogMessage << def << std::endl;

    WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logLastMessages.push_front(sLogMessage);


    while (WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logLastMessages.size() > 50) {
        WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logLastMessages.pop_back();
    }

    // log file 
    if (WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.enableLogFile) {
        std::ofstream logFile(WsjcppLog::g_WSJCPP_LOG_GLOBAL_CONF.logFile, std::ios::app);
        if (!logFile) {
            std::cout << "Error Opening File" << std::endl;
            return;
        }

        logFile << sLogMessage << std::endl;
        logFile.close();    
    }
}

// ---------------------------------------------------------------------
// WsjcppResourceFile

WsjcppResourceFile::WsjcppResourceFile() {
    WsjcppResourcesManager::add(this);
}

// ---------------------------------------------------------------------


// ---------------------------------------------------------------------
// WsjcppResourcesManager

std::vector<WsjcppResourceFile*> *g_pWsjcppResourceFiles = nullptr;

void WsjcppResourcesManager::initGlobalVariables() {
    if (g_pWsjcppResourceFiles == nullptr) {
        g_pWsjcppResourceFiles = new std::vector<WsjcppResourceFile*>();
    }
}

// ---------------------------------------------------------------------

void WsjcppResourcesManager::add(WsjcppResourceFile* pStorage) {
    WsjcppResourcesManager::initGlobalVariables();
    g_pWsjcppResourceFiles->push_back(pStorage);
}

// ---------------------------------------------------------------------

bool WsjcppResourcesManager::has(const std::string &sFilename) {
    WsjcppResourcesManager::initGlobalVariables();
    for (int i = 0; i < WsjcppResourcesManager::list().size(); i++) {
        if (WsjcppResourcesManager::list()[i]->getFilename() == sFilename) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

WsjcppResourceFile* WsjcppResourcesManager::get(const std::string &sFilename) {
    WsjcppResourcesManager::initGlobalVariables();
    for (int i = 0; i < WsjcppResourcesManager::list().size(); i++) {
        if (WsjcppResourcesManager::list()[i]->getFilename() == sFilename) {
            return WsjcppResourcesManager::list()[i];
        }
    }
    return nullptr;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppResourceFile*> &WsjcppResourcesManager::list() {
    return *g_pWsjcppResourceFiles;
}

// ---------------------------------------------------------------------

/*
bool WsjcppResourcesManager::make(const std::string &sWorkspace) {
    if (!WsjcppResourcesManager::createFolders(sWorkspace)) {
        return false;
    }
    return WsjcppResourcesManager::extractFiles(sWorkspace);
}

// ---------------------------------------------------------------------

bool WsjcppResourcesManager::createFolders(const std::string &sWorkspace) {
    // prepare folders
    std::vector<std::string> vCreateDirs;
    vCreateDirs.push_back(sWorkspace + "/logs");
    vCreateDirs.push_back(sWorkspace + "/teams");
    vCreateDirs.push_back(sWorkspace + "/checkers");
    vCreateDirs.push_back(sWorkspace + "/html");
    vCreateDirs.push_back(sWorkspace + "/html/css");
    vCreateDirs.push_back(sWorkspace + "/html/js");
    vCreateDirs.push_back(sWorkspace + "/html/images");
    vCreateDirs.push_back(sWorkspace + "/html/images/teams");
    vCreateDirs.push_back(sWorkspace + "/html/images/states");

    for(int i = 0; i < vCreateDirs.size(); i++) {
        std::string sPath = vCreateDirs[i];
        // check dir existing
        if (!FS::dirExists(sPath)) {
            // try make dir
            if (!FS::makeDir(sPath)) {
                std::cout << "Could not create folder " << sPath << std::endl;
                return false;
            } else {
                std::cout << "Created folder " << sPath << std::endl;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppResourcesManager::extractFiles(const std::string &sWorkspace) {
    // TODO mkdir -p for files
    const std::vector<WsjcppResourceFile*> list = WsjcppResourcesManager::list();
    for(int i = 0; i < list.size(); i++) {
        std::string sFilename = sWorkspace + "/" + list[i]->filename();
        if (!FS::fileExists(sFilename)) {
            if (!FS::writeFile(sFilename, list[i]->buffer(), list[i]->bufferSize())) {
                std::cout << "Could not write file " << sFilename << std::endl;
                return false;
            } else {
                std::cout << "Created file " << sFilename << std::endl;
            }
        }
    }
    return true;
}
*/

