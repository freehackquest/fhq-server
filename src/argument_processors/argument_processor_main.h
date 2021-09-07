#ifndef ARGUMENT_PROCESSOR_MAIN_H
#define ARGUMENT_PROCESSOR_MAIN_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorMain : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorMain();

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue) override;
        virtual bool applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) override;
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;

        bool setWorkDir(const std::string &sWorkDir);
        const std::string &getWorkDir();

    private:
        std::string m_sWorkDir;
};

class ArgumentProcessorVersion : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorVersion();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

#endif // ARGUMENT_PROCESSOR_MAIN_H
