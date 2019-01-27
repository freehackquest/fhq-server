#ifndef CMD_HANDLERS_QUESTS_WRITEUPS_H
#define CMD_HANDLERS_QUESTS_WRITEUPS_H

#include <cmd_handlers.h>

// *******************************************
// ************ Quests WriteUps **************
// *******************************************

class CmdHandlerQuestsWriteUps : public CmdHandlerBase {
    public:
        CmdHandlerQuestsWriteUps();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HANDLERS_QUESTS_WRITEUPS_H
