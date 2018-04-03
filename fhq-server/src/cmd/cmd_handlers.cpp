#include <log.h>
#include <cmd_handlers.h>

// ---------------------------------------------------------------------

std::map<std::string, ICmdHandler*> *g_pCmdHandlers = NULL;

ICmdHandler * findCmdHandler(const std::string &cmd){
    ICmdHandler *pCmdHandler = NULL;

    if(g_pCmdHandlers == NULL){
        return NULL;
    }

    if(g_pCmdHandlers->count(cmd)){
        pCmdHandler = g_pCmdHandlers->at(cmd);
    }

    if(pCmdHandler == NULL){
        Log::err("findCmdHandler", "Not found cmd_handler " + QString(cmd.c_str()));
    }

    return pCmdHandler;
}


// ---------------------------------------------------------------------

