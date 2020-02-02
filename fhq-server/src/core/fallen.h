#ifndef FALLEN_H
#define FALLEN_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <json.hpp>
#include <deque>
#include <wsjcpp_core.h>

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
