#ifndef ARGUMENT_PROCESSOR_MAIN_H
#define ARGUMENT_PROCESSOR_MAIN_H

#include <QtCore>
#include <wsjcpp_arguments.h>

class ArgumentProcessorMain : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorMain(QCoreApplication *pQtApp);

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue) override;
        virtual bool applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) override;
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;

        bool setWorkDir(const std::string &sWorkDir);
        bool initDefaultDataForContainers(const std::string &sInitDir);

    private:
        std::string m_sWorkDir;

        void initDefaultDataConfigYml(const std::string &sNormalizedInitDir);
        void initDefaultSsl(const std::string &sNormalizedInitDir);
        void initDefaultNginx(const std::string &sNormalizedInitDir);
};

class ArgumentProcessorVersion : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorVersion();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorPrepareDeb : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorPrepareDeb();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorShowEmployees : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorShowEmployees();
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorStart : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorStart(QCoreApplication *pQtApp);
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
    private:
        QCoreApplication *m_pQtApp;
};

#endif // ARGUMENT_PROCESSOR_MAIN_H
