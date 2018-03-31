#ifndef EMPLOY_JSON_H
#define EMPLOY_JSON_H

#include <employees.h>

class EmployJson : public IEmploy {
    public:
       static std::string getEmployName() { return "EmployJson"; }
       virtual void test();
};

#endif // EMPLOY_JSON_H
