#ifndef WSJCPP_CORE_H
#define WSJCPP_CORE_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <deque>
#include <iostream>

class WsjcppCore {
    public:
        static bool init(
            int argc, char** argv, 
            const std::string &sApplicationName,
            const std::string &sApplicationVersion,
            const std::string &sApplicationAuthor,
            const std::string &sLibraryNameForExports
        );

        static std::string doNormalizePath(const std::string &sPath);
        static std::string extractFilename(const std::string &sPath);
        static std::string getCurrentDirectory();

        static long currentTime_milliseconds();
        static long currentTime_seconds();
        static std::string currentTime_forFilename();
        static std::string currentTime_logformat();
        static std::string threadId();
        static std::string formatTimeForWeb(long nTimeInSec);
        static std::string formatTimeForFilename(long nTimeInSec);
        static std::string formatTimeUTC(int nTimeInSec);

        static bool dirExists(const std::string &sFilename);
        static bool fileExists(const std::string &sFilename);
        static std::vector<std::string> listOfDirs(const std::string &sDirname);
        static std::vector<std::string> listOfFiles(const std::string &sDirname);     
        static bool makeDir(const std::string &sDirname);
        static bool writeFile(const std::string &sFilename, const std::string &sContent);
        static bool readTextFile(const std::string &sFilename, std::string &sOutputContent);
        static bool readFileToBuffer(const std::string &sFilename, char *pBuffer[], int &nBufferSize);
        static bool writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize);
        static bool removeFile(const std::string &sFilename);
        static bool createEmptyFile(const std::string &sFilename);

        static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string toLower(const std::string &str);
        static std::string toUpper(const std::string& str);
        static void replaceAll(std::string& str, const std::string& from, const std::string& to);
        static std::vector<std::string> split(const std::string& sWhat, const std::string& sDelim);
        static std::string join(const std::vector<std::string> &vWhat, const std::string& sDelim);

        static void initRandom();
        static std::string createUuid();
        static std::string uint2hexString(unsigned int n);
        static unsigned long convertVoidToULong(void *p);
        static std::string getPointerAsHex(void *p);
        static std::string extractURLProtocol(const std::string& sValue);
        static bool getEnv(const std::string& sName, std::string& sValue);

        static std::string encodeUriComponent(const std::string& sValue);
        static std::string decodeUriComponent(const std::string& sValue);

        static std::string getHumanSizeBytes(long nBytes);
};


// ---------------------------------------------------------------------

enum WsjcppColorCode {
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_YELLOW   = 93,
    FG_BLUE     = 34,
    FG_DEFAULT  = 39,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_BLUE     = 44,
    BG_DEFAULT  = 49
};

// ---------------------------------------------------------------------

class WsjcppColorModifier {
    WsjcppColorCode code;
    public:
        WsjcppColorModifier(WsjcppColorCode pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const WsjcppColorModifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
};

// ---------------------------------------------------------------------

class WsjcppLog {
    public:
        static std::string g_WSJCPP_LOG_DIR;
        static std::string g_WSJCPP_LOG_PREFIX_FILE;
        static std::string g_WSJCPP_LOG_FILE;
        static long g_WSJCPP_LOG_START_TIME;
        static std::mutex * g_WSJCPP_LOG_MUTEX;
        static std::deque<std::string> * g_WSJCPP_LOG_LAST_MESSAGES;
        static void doLogRotateUpdateFilename(bool bForce = false);

        static void info(const std::string &sTag, const std::string &sMessage);
        static void err(const std::string &sTag, const std::string &sMessage);
        static void throw_err(const std::string &sTag, const std::string &sMessage);
        static void warn(const std::string &sTag, const std::string &sMessage);
        static void ok(const std::string &sTag, const std::string &sMessage);
        static std::vector<std::string> getLastLogMessages();
        static void setLogDirectory(const std::string &sDirectoryPath);
        static void setPrefixLogFile(const std::string &sPrefixLogFile);
        static void initGlobalVariables();

    private:
        static void add(WsjcppColorModifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage);
};

#endif // WSJCPP_CORE_H


