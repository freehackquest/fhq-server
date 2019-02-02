#ifndef CMD_HANDLERS_QUESTS_WRITEUPS_H
#define CMD_HANDLERS_QUESTS_WRITEUPS_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------
// Quests WriteUps

class CmdHandlerQuestsWriteUpsList : public CmdHandlerBase {
    public:
        CmdHandlerQuestsWriteUpsList();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Quests WriteUps Proposal

class CmdHandlerQuestsWriteUpsProposal : public CmdHandlerBase {
    public:
        CmdHandlerQuestsWriteUpsProposal();
        virtual void handle(ModelRequest *pRequest);
    private:
        std::string m_sLinkPrefix;
};

// ---------------------------------------------------------------------
// Quests WriteUps Update

class CmdHandlerQuestsWriteUpsUpdate : public CmdHandlerBase {
    public:
        CmdHandlerQuestsWriteUpsUpdate();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Quests WriteUps Delete

class CmdHandlerQuestsWriteUpsDelete : public CmdHandlerBase {
    public:
        CmdHandlerQuestsWriteUpsDelete();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HANDLERS_QUESTS_WRITEUPS_H
