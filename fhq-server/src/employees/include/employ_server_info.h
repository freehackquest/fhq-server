#ifndef EMPLOY_SERVER_INFO_H
#define EMPLOY_SERVER_INFO_H

#include <employees.h>
#include <employ_base.h>

class EmployServerInfo : public IEmploy, EmployBase {
    public:
        EmployServerInfo();
       static std::string getEmployName() { return "EmployJson"; }
       virtual void test();
};

#endif // EMPLOY_SERVER_INFO_H
