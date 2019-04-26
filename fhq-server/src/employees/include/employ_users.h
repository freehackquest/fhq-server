#ifndef EMPLOY_GAMES_H
#define EMPLOY_GAMES_H

#include <employees.h>

class EmployUsers : public EmployBase {
    public:
        EmployUsers();
        static std::string name() { return "EmployUsers"; }
        virtual bool init();
        
        // TODO employee 

    private:
        std::string TAG;
};

#endif // EMPLOY_GAMES_H
