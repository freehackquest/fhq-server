#ifndef EMPLOY_CHATS_H
#define EMPLOY_CHATS_H

#include <wsjcpp_employees.h>
#include <model_leak.h>

class EmployChats : public WSJCppEmployBase {
    public:
        EmployChats();
        static std::string name() { return "EmployChats"; }
        virtual bool init();

    private:
        std::string TAG;
};

#endif // EMPLOY_CHATS_H
