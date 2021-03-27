#ifndef WSJCPP_CORE_H
#define WSJCPP_CORE_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <deque>
#include <iostream>

class WsjcppFilePermissions {
    public:
        WsjcppFilePermissions();
        WsjcppFilePermissions(
            bool bOwnerReadFlag, bool bOwnerWriteFlag, bool bOwnerExecuteFlag,
            bool bGroupReadFlag, bool bGroupWriteFlag, bool bGroupExecuteFlag,
            bool bOtherReadFlag, bool bOtherWriteFlag, bool bOtherExecuteFlag
        );
        WsjcppFilePermissions(uint16_t nFilePermission);

        // owner flags
        void setOwnerReadFlag(bool bOwnerReadFlag);
        bool getOwnerReadFlag() const;
        void setOwnerWriteFlag(bool bOwnerWriteFlag);
        bool getOwnerWriteFlag() const;
        void setOwnerExecuteFlag(bool bOwnerExecuteFlag);
        bool getOwnerExecuteFlag() const;
        void setOwnerFlags(bool bOwnerReadFlag, bool bOwnerWriteFlag, bool bOwnerExecuteFlag);

        // group flags
        void setGroupReadFlag(bool bGroupReadFlag);
        bool getGroupReadFlag() const;
        void setGroupWriteFlag(bool bGroupWriteFlag);
        bool getGroupWriteFlag() const;
        void setGroupExecuteFlag(bool bGroupExecuteFlag);
        bool getGroupExecuteFlag() const;
        void setGroupFlags(bool bGroupReadFlag, bool bGroupWriteFlag, bool bGroupExecuteFlag);

        // for other flags
        void setOtherReadFlag(bool bOtherReadFlag);
        bool getOtherReadFlag() const;
        void setOtherWriteFlag(bool bOtherWriteFlag);
        bool getOtherWriteFlag() const;
        void setOtherExecuteFlag(bool bOtherExecuteFlag);
        bool getOtherExecuteFlag() const;
        void setOtherFlags(bool bOtherReadFlag, bool bOtherWriteFlag, bool bOtherExecuteFlag);

        std::string toString() const;
        uint16_t toUInt16() const;

    private:
        bool m_bOwnerReadFlag;
        bool m_bOwnerWriteFlag;
        bool m_bOwnerExecuteFlag;
        bool m_bGroupReadFlag;
        bool m_bGroupWriteFlag;
        bool m_bGroupExecuteFlag;
        bool m_bOtherReadFlag;
        bool m_bOtherWriteFlag;
        bool m_bOtherExecuteFlag;
};

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

        static long getCurrentTimeInMilliseconds();
        static long getCurrentTimeInSeconds();
        static std::string getCurrentTimeForFilename();
        static std::string getCurrentTimeForLogFormat();
        static std::string getThreadId();
        static std::string formatTimeForWeb(long nTimeInSec);
        static std::string formatTimeForFilename(long nTimeInSec);
        static std::string formatTimeUTC(int nTimeInSec);

        static bool dirExists(const std::string &sFilename);
        static bool fileExists(const std::string &sFilename);
        static std::vector<std::string> listOfDirs(const std::string &sDirname);
        static std::vector<std::string> getListOfDirs(const std::string &sDirname);
        static std::vector<std::string> listOfFiles(const std::string &sDirname);
        static std::vector<std::string> getListOfFiles(const std::string &sDirname);
        static bool makeDir(const std::string &sDirname);
        static bool writeFile(const std::string &sFilename, const std::string &sContent);
        static bool readTextFile(const std::string &sFilename, std::string &sOutputContent);
        static bool readFileToBuffer(const std::string &sFilename, char *pBuffer[], int &nBufferSize);
        static bool writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize);
        static bool removeFile(const std::string &sFilename);
        static bool copyFile(const std::string &sSourceFilename, const std::string &sTargetFilename);

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

        static bool recoursiveCopyFiles(const std::string& sSourceDir, const std::string& sTargetDir);
        static bool recoursiveRemoveDir(const std::string& sDir);

        static bool setFilePermissions(const std::string& sFilePath, const WsjcppFilePermissions &filePermissions, std::string& sError);
        static bool getFilePermissions(const std::string& sFilePath, WsjcppFilePermissions &filePermissions, std::string& sError);

        static std::string doPadLeft(const std::string& sIn, char cWhat, int nLength);
        static std::string doPadRight(const std::string& sIn, char cWhat, int nLength);

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

class WsjcppLogGlobalConf {
    public:
        WsjcppLogGlobalConf();
        void doLogRotateUpdateFilename(bool bForce = false);
        std::mutex logMutex;
        std::string logDir;
        std::string logPrefixFile;
        std::string logFile;
        bool enableLogFile;
        long logStartTime;
        long logRotationPeriodInSeconds;
        std::deque<std::string> logLastMessages;
};

class WsjcppLog {
    public:
        static WsjcppLogGlobalConf g_WSJCPP_LOG_GLOBAL_CONF;

        static void info(const std::string &sTag, const std::string &sMessage);
        static void err(const std::string &sTag, const std::string &sMessage);
        static void throw_err(const std::string &sTag, const std::string &sMessage);
        static void warn(const std::string &sTag, const std::string &sMessage);
        static void ok(const std::string &sTag, const std::string &sMessage);
        static std::vector<std::string> getLastLogMessages();
        static void setLogDirectory(const std::string &sDirectoryPath);
        static void setPrefixLogFile(const std::string &sPrefixLogFile);
        static void setEnableLogFile(bool bEnable);
        static void setRotationPeriodInSec(long nRotationPeriodInSec);

    private:
        static void add(WsjcppColorModifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage);
};

// ---------------------------------------------------------------------
// WsjcppResourceFile

class WsjcppResourceFile {
    public:
        WsjcppResourceFile();
        virtual const std::string &getFilename() const = 0;
        virtual const std::string &getPackAs() const = 0;
        virtual int getBufferSize() const = 0;
        virtual const char *getBuffer() const = 0;
};


// ---------------------------------------------------------------------
// WsjcppResourcesManager

extern std::vector<WsjcppResourceFile*> *g_pWsjcppResourceFiles;

class WsjcppResourcesManager {
    public:
        static void initGlobalVariables();
        static void add(WsjcppResourceFile*);
        static const std::vector<WsjcppResourceFile*> &list();
        static bool has(const std::string &sFilename);
        static WsjcppResourceFile* get(const std::string &sFilename);
        static bool make(const std::string &sWorkspace);
        // static bool createFolders(const std::string &sWorkspace);
        // static bool extractFiles(const std::string &sWorkspace);
};

// ---------------------------------------------------------------------
// Registry WsjcppResourceFile
#define REGISTRY_WSJCPP_RESOURCE_FILE( classname ) \
    static classname * pRegistryWsjcppResourceFile ## classname = new classname(); \


#endif // WSJCPP_CORE_H


