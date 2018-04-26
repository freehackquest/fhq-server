#ifndef EMPLOY_WS_SERVER_H
#define EMPLOY_WS_SERVER_H

#include <employees.h>
#include <errors.h>
#include <cmd_handlers.h>
// 
#include <QJsonObject>

class EmployWsServer : public EmployBase {
    public:
       EmployWsServer();
       static std::string name() { return "EmployWsServer"; }
       virtual bool init();
       bool validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler, QJsonObject &jsonRequest);

   private:
		std::string TAG;
};

#endif // EMPLOY_WS_SERVER_H
