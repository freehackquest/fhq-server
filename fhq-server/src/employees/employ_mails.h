#ifndef EMPLOY_MAILS_H
#define EMPLOY_MAILS_H

#include <wsjcpp_employees.h>

class EmployMails : public WSJCppEmployBase {
    public:
        EmployMails();
        static std::string name() { return "EmployMails"; }
        virtual bool init();

    private:
        std::string TAG;
};

#endif // EMPLOY_MAILS_H
