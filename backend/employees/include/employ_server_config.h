#ifndef EMPLOY_SERVER_CONFIG_H
#define EMPLOY_SERVER_CONFIG_H

#include <employees.h>

class EmployServerConfig : public IEmploy {
    public:
       static std::string getEmployName() { return "EmployServerConfig"; }
       virtual void test();
};

REGISTRY_EMPLOY(EmployServerConfig)

#endif // EMPLOY_SERVER_CONFIG_H
