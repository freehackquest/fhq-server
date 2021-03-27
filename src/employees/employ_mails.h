#ifndef EMPLOY_MAILS_H
#define EMPLOY_MAILS_H

#include <employees.h>

class EmployMails : public WsjcppEmployBase {
    public:
        EmployMails();
        static std::string name() { return "EmployMails"; }
        virtual bool init();
        virtual bool deinit();

    private:
        std::string TAG;
};

#endif // EMPLOY_MAILS_H
