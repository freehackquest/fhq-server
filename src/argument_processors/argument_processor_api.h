#ifndef ARGUMENT_PROCESSOR_API_H
#define ARGUMENT_PROCESSOR_API_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorApi : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorApi();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorApiList : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorApiList();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorApiExportPythonLibrary : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorApiExportPythonLibrary();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorApiExportJavascriptLibrary : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorApiExportJavascriptLibrary();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorApiExportJavaAndroidLibrary : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorApiExportJavaAndroidLibrary();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

#endif // ARGUMENT_PROCESSOR_API_H
