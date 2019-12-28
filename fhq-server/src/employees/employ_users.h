#ifndef EMPLOY_USERS_H
#define EMPLOY_USERS_H

#include <wsjcpp_employees.h>

class EmployUsers : public WSJCppEmployBase {
    public:
        EmployUsers();
        static std::string name() { return "EmployUsers"; }
        virtual bool init();
        
        // TODO employee 

    private:
        std::string TAG;
};

#endif // EMPLOY_USERS_H
