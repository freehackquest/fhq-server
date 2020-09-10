#ifndef CMD_HANDLERS_QUESTS_WRITEUPS_H
#define CMD_HANDLERS_QUESTS_WRITEUPS_H

#include <wsjcpp_jsonrpc20.h>

// ---------------------------------------------------------------------
// Quests WriteUps

class CmdHandlerQuestsWriteUpsList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestsWriteUpsList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Quests WriteUps Proposal

class CmdHandlerQuestsWriteUpsProposal : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestsWriteUpsProposal();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
    private:
        std::string m_sLinkPrefix;
};

// ---------------------------------------------------------------------
// Quests WriteUps Update

class CmdHandlerQuestsWriteUpsUpdate : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestsWriteUpsUpdate();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Quests WriteUps Delete

class CmdHandlerQuestsWriteUpsDelete : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestsWriteUpsDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

#endif // CMD_HANDLERS_QUESTS_WRITEUPS_H
