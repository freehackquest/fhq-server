#ifndef EMPLOY_USERS_H
#define EMPLOY_USERS_H

#include <employees.h>

class EmployUsers : public WsjcppEmployBase {
    public:
        EmployUsers();
        static std::string name() { return "EmployUsers"; }
        virtual bool init();
        virtual bool deinit() override;
        
        // TODO employee 

    private:
        std::string TAG;
};

#endif // EMPLOY_USERS_H
