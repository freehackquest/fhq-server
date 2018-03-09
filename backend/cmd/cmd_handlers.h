#ifndef CMD_HADNLERS_H
#define CMD_HADNLERS_H

#include <icmdhandler.h>
#include <log.h>
#include <map>

extern std::map<std::string, ICmdHandler*> *g_pCmdHandlers;
extern ICmdHandler * findCmdHandler(const std::string &cmd);

// RegistryCmdHandler

template <typename T> class RegistryCmdHandler {
public:
    RegistryCmdHandler() {
        if(g_pCmdHandlers == NULL){
            g_pCmdHandlers = new std::map<std::string, ICmdHandler*>();
        }

        ICmdHandler *pCmdHandler = new T();
        std::string sCmd = pCmdHandler->cmd(); // TODO change to static method
        std::cout << "Try register cmd: " << sCmd << "\n";
        g_pCmdHandlers->insert(std::pair<std::string, ICmdHandler*>(sCmd,pCmdHandler));
    }
};

#define REGISTRY_CMD( classname ) \
    RegistryCmdHandler<classname> registryCmdHandler ## classname;


#endif // CMD_HADNLERS_H
