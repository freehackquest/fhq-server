#ifndef CMD_HANDLERS_QUESTS_H
#define CMD_HANDLERS_QUESTS_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerQuests : public CmdHandlerBase {
    public:
        CmdHandlerQuests();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuest : public CmdHandlerBase {
    public:
        CmdHandlerQuest();
        virtual void handle(ModelRequest *pRequest);

};

// ---------------------------------------------------------------------

class CmdHandlerQuestPass : public CmdHandlerBase {
    public:
        CmdHandlerQuestPass();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerCreateQuest : public CmdHandlerBase {
    public:
        CmdHandlerCreateQuest();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestDelete : public CmdHandlerBase {
    public:
        CmdHandlerQuestDelete();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestProposal : public CmdHandlerBase {
    public:
        CmdHandlerQuestProposal();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestStatistics : public CmdHandlerBase {
    public:
        CmdHandlerQuestStatistics();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestUpdate : public CmdHandlerBase {
    public:
        CmdHandlerQuestUpdate();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestsSubjects : public CmdHandlerBase {
    public:
        CmdHandlerQuestsSubjects();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerAddHint : public CmdHandlerBase {
    public:
        CmdHandlerAddHint();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerAnswerList : public CmdHandlerBase {
    public:
        CmdHandlerAnswerList();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestsUserAnswerList : public CmdHandlerBase {
    public:
        CmdHandlerQuestsUserAnswerList();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerDeleteHint : public CmdHandlerBase {
    public:
        CmdHandlerDeleteHint();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerHints : public CmdHandlerBase {
    public:
        CmdHandlerHints();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestsProposalList : public CmdHandlerBase {
    public:
        CmdHandlerQuestsProposalList();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestsFilesUpload : public CmdHandlerBase {
    public:
        CmdHandlerQuestsFilesUpload();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerQuestsFilesDelete : public CmdHandlerBase {
    public:
        CmdHandlerQuestsFilesDelete();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HANDLERS_QUESTS_H
