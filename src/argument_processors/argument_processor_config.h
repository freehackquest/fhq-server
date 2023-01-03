#ifndef ARGUMENT_PROCESSOR_CONFIG_H
#define ARGUMENT_PROCESSOR_CONFIG_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorConfig : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfig();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigTest : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigTest();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigPrint : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigPrint();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigTestEmail : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigTestEmail();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigSet : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigSet();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigLxdEnable : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigLxdEnable();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigLxdDisable : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigLxdDisable();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigLxdConfigure : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigLxdConfigure();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

#endif // ARGUMENT_PROCESSOR_CONFIG_H
