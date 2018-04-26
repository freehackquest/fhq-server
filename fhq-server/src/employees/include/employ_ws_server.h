#ifndef EMPLOY_WS_SERVER_H
#define EMPLOY_WS_SERVER_H

#include <employees.h>

class EmployWsServer : public EmployBase {
    public:
       EmployWsServer();
       static std::string name() { return "EmployWsServer"; }
       virtual bool init();

   private:
		std::string TAG;
};

#endif // EMPLOY_WS_SERVER_H
