#ifndef FALLEN_H
#define FALLEN_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <json.hpp>
#include <deque>

class Fallen {
    public:
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
        static bool writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize);

        static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& to_lower(std::string& str);

        static void initRandom();
        static std::string createUuid();
        static unsigned long convertVoid2Int(void *p);
        static std::string getPointerAsHex(void *p);
        static std::string doNormalizePath(const std::string & sPath);

};

// ---------------------------------------------------------------------

enum FallenHelpParseArgType {
    PARAMETER,
    SINGLE_OPTION
};

// ---------------------------------------------------------------------

class FallenHelpParseArg {
    public:
        // TODO redesign to alias list {"1", "2"}
        FallenHelpParseArg(const std::string &sName, const std::string &sAlias, FallenHelpParseArgType nType, const std::string &sDescription);
        FallenHelpParseArg();
        std::string name();
        std::string alias();
        std::string description();
        bool isOption();

    private:
        std::string m_sName;
        std::string m_sAlias;
        std::string m_sDescription;
        bool m_bIsOption;
};

// ---------------------------------------------------------------------

class FallenHelpParseArgs {
    public:
        FallenHelpParseArgs(int argc, char** argv);
        void setAppName(const std::string &sAppName);
        void setAppVersion(const std::string &sAppVersion);
        bool has(const std::string &sName);
        std::string option(const std::string &sName);
        void addHelp(const std::string &sName, const std::string &sAlias, FallenHelpParseArgType nType, const std::string &sDescription);
        void printHelp();
        bool checkArgs(std::string &sArgsErrors);

    private:
        std::vector<std::string> m_vArgs;
        std::vector<FallenHelpParseArg *> m_vHelpArgs;
        std::string m_sAppName;
        std::string m_sAppVersion;
};

// ---------------------------------------------------------------------

namespace Color {
    enum Code {
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
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
};

// ---------------------------------------------------------------------

class Log {
    public:
        static std::string g_LOG_DIR;
        static std::string g_PREFIX_LOG_FILE;
        static std::string g_LOG_FILE;
        static long g_LOG_START_TIME;
        static void doLogRotateUpdateFilename(bool bForce = false);

        static void info(const std::string &sTag, const std::string &sMessage);
        static void err(const std::string &sTag, const std::string &sMessage);
        static void throw_err(const std::string &sTag, const std::string &sMessage);
        static void warn(const std::string &sTag, const std::string &sMessage);
        static void ok(const std::string &sTag, const std::string &sMessage);
        static void setLogDirectory(const std::string &sDirectoryPath);
        static void setPrefixLogFile(const std::string &sPrefixLogFile);
        static nlohmann::json getLastLogs();
        static void initGlobalVariables();

    private:
        static void add(Color::Modifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage);
};

// TODO redesign to extern

extern std::mutex *g_LOG_MUTEX;
extern std::deque<std::string> *g_LAST_LOG_MESSAGES;

// ---------------------------------------------------------------------

class WJSCppParseConfig {

    public:
        WJSCppParseConfig(const std::string &sFilepathConf);
        bool load();

        bool has(const std::string &sParamName);
        std::string stringValue(const std::string &sParamName, const std::string &defaultValue);
        int intValue(const std::string &sParamName, int defaultValue);
        bool boolValue(const std::string &sParamName, bool defaultValue);
        std::map<std::string,std::string> getValues();
    private:
        std::string TAG;
        std::string m_sFilepathConf;
        std::map<std::string,std::string> m_mapConfigValues;
};

#endif // FALLEN_H
