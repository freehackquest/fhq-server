#ifndef EMPLOY_CHATS_H
#define EMPLOY_CHATS_H

#include <wjscpp_employees.h>
#include <model_leak.h>

class EmployChats : public EmployBase {
    public:
        EmployChats();
        static std::string name() { return "EmployChats"; }
        virtual bool init();

    private:
        std::string TAG;
};

#endif // EMPLOY_CHATS_H
