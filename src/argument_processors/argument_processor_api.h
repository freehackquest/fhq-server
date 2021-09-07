#ifndef ARGUMENT_PROCESSOR_API_H
#define ARGUMENT_PROCESSOR_API_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorApi : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorApi();

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue) override;
        virtual bool applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) override;
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorApiList : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorApiList();

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue) override;
        virtual bool applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) override;
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

#endif // ARGUMENT_PROCESSOR_API_H
