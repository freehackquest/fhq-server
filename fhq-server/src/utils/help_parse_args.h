#ifndef HELP_PARSE_ARGS_H
#define HELP_PARSE_ARGS_H

#include <string>
#include <vector>
#include <map>

// ---------------------------------------------------------------------

enum HelpParseArgType {
    PARAMETER,
    SINGLE_OPTION
};

// ---------------------------------------------------------------------

class HelpParseArg {
    public:
        // TODO redesign to alias list {"1", "2"}
        HelpParseArg(const std::string &sName, const std::string &sAlias, HelpParseArgType nType, const std::string &sDescription);
        HelpParseArg();
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

class HelpParseArgs {
    public:
        HelpParseArgs(int argc, char** argv);
        void setAppName(const std::string &sAppName);
        void setAppVersion(const std::string &sAppVersion);
        bool has(const std::string &sName);
        std::string option(const std::string &sName);
        void addHelp(const std::string &sName, const std::string &sAlias, HelpParseArgType nType, const std::string &sDescription);
        void printHelp();
        bool checkArgs(std::string &sArgsErrors);

    private:
        std::vector<std::string> m_vArgs;
        std::vector<HelpParseArg *> m_vHelpArgs;
        std::string m_sAppName;
        std::string m_sAppVersion;
};

#endif // HELP_PARSE_ARGS_H
