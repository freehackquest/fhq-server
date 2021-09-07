#ifndef ARGUMENT_PROCESSOR_DATABASE_H
#define ARGUMENT_PROCESSOR_DATABASE_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorDatabase : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorDatabase();

        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorDatabaseTestConnection : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorDatabaseTestConnection();

        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

#endif // ARGUMENT_PROCESSOR_DATABASE_H
