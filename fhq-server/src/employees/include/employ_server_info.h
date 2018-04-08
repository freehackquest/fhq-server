#ifndef EMPLOY_SERVER_INFO_H
#define EMPLOY_SERVER_INFO_H

#include <employees.h>

class EmployServerInfo : public EmployBase {
    public:
       EmployServerInfo();
       static std::string name() { return "EmployServerInfo"; }
       virtual bool init();
};

#endif // EMPLOY_SERVER_INFO_H
