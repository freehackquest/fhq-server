#ifndef EMPLOY_CHATS_H
#define EMPLOY_CHATS_H

#include <employees.h>
#include <model_leak.h>

class EmployChats : public WsjcppEmployBase {
    public:
        EmployChats();
        static std::string name() { return "EmployChats"; }
        virtual bool init();
        virtual bool deinit();

    private:
        std::string TAG;
};

#endif // EMPLOY_CHATS_H
