#ifndef EMPLOY_NOTIFY_H
#define EMPLOY_NOTIFY_H

#include <employees.h>
#include <model_notification.h>

class EmployNotify : public EmployBase {
    public:

        EmployNotify();
        static std::string name() { return "EmployNotify"; }
        virtual bool init();

        void sendNotification(ModelNotification &modelNotification);

    private:
        std::string TAG;

};

#endif // EMPLOY_NOTIFY_H
