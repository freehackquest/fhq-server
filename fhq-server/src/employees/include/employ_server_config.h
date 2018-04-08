#ifndef EMPLOY_SERVER_CONFIG_H
#define EMPLOY_SERVER_CONFIG_H

#include <employees.h>

class EmployServerConfig : public EmployBase {
    public:
        EmployServerConfig();
        static std::string name() { return "EmployServerConfig"; }
        virtual bool init();
};

#endif // EMPLOY_SERVER_CONFIG_H
