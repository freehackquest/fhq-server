#ifndef EMPLOY_NOTIFY_H
#define EMPLOY_NOTIFY_H

#include <wsjcpp_employees.h>
#include <model_notification.h>

class EmployNotify : public WSJCppEmployBase {
    public:

        EmployNotify();
        static std::string name() { return "EmployNotify"; }
        virtual bool init();
        static std::string GAMES;
        static std::string QUESTS;
        static std::string USERS;
        static std::string SCOREBOARD;
        static std::string LEAKS;

        void sendNotification(ModelNotification &modelNotification); // wrong

        void notifyWarning(const std::string &sSection, const std::string &sMessage);
        void notifyDanger(const std::string &sSection, const std::string &sMessage);
        void notifyInfo(const std::string &sSection, const std::string &sMessage);
        void notifySuccess(const std::string &sSection, const std::string &sMessage);

    private:
        void sendNotification(const std::string &sType,
                              const std::string &sSection,
                              const std::string &sMessage);
        std::string TAG;

};

#endif // EMPLOY_NOTIFY_H
