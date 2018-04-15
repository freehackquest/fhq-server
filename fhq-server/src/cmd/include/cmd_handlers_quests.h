#ifndef CMD_HANDLERS_QUESTS_H
#define CMD_HANDLERS_QUESTS_H

#include <cmd_handlers.h>

// *********************************************
// *************** List of Quests **************
// *********************************************

class CmdHandlerQuests : public CmdHandlerBase {
	public:
        CmdHandlerQuests();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuests)

// *********************************************
// *************** Quest Info ******************
// *********************************************

class CmdHandlerQuest : public CmdHandlerBase {
    public:
        CmdHandlerQuest();
        virtual void handle(ModelRequest *pRequest);

};

REGISTRY_CMD(CmdHandlerQuest)


// *******************************************
// *************** Quest Pass ****************
// *******************************************

class CmdHandlerQuestPass : public CmdHandlerBase {
    public:
        CmdHandlerQuestPass();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuestPass)

// *******************************************
// ************** Quest Create ***************
// *******************************************

class CmdHandlerCreateQuest : public CmdHandlerBase {
    public:
        CmdHandlerCreateQuest();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerCreateQuest)

// *******************************************
// ************** Quest Delete ***************
// *******************************************

class CmdHandlerQuestDelete : public CmdHandlerBase {
    public:
        CmdHandlerQuestDelete();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuestDelete)

// *******************************************
// ************** Quest Proposal ***************
// *******************************************

class CmdHandlerQuestProposal : public CmdHandlerBase {
    public:
        CmdHandlerQuestProposal();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuestProposal)


// *******************************************
// ************** Quest Statistics ***********
// *******************************************

class CmdHandlerQuestStatistics : public CmdHandlerBase {
    public:
        CmdHandlerQuestStatistics();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuestStatistics)

// *******************************************
// ************** Quest Update ***********
// *******************************************

class CmdHandlerQuestUpdate : public CmdHandlerBase {
    public:
        CmdHandlerQuestUpdate();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuestUpdate)


// *******************************************
// ************** Quests Subjects ************
// *******************************************

class CmdHandlerQuestsSubjects : public CmdHandlerBase {
    public:
        CmdHandlerQuestsSubjects();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuestsSubjects)

// *******************************************
// ************ Quest Hints Add **************
// *******************************************

class CmdHandlerAddHint : public CmdHandlerBase {
    public:
        CmdHandlerAddHint();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerAddHint)

// *******************************************
// ************ Quest AnswerList **************
// *******************************************

class CmdHandlerAnswerList : public CmdHandlerBase {
    public:
        CmdHandlerAnswerList();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerAnswerList)

// *******************************************
// *********** Quest Hints Delete ************
// *******************************************

class CmdHandlerDeleteHint : public CmdHandlerBase {
    public:
        CmdHandlerDeleteHint();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerDeleteHint)

// *******************************************
// **************** Quest Hints **************
// *******************************************

class CmdHandlerHints : public CmdHandlerBase {
    public:
        CmdHandlerHints();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerHints)

// *******************************************
// ********* Quests Proposal List ************
// *******************************************

class CmdHandlerQuestsProposalList : public CmdHandlerBase {
    public:
        CmdHandlerQuestsProposalList();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuestsProposalList)

// *******************************************
// ************ Quests WriteUps **************
// *******************************************

class CmdHandlerWriteUps : public CmdHandlerBase {
    public:
        CmdHandlerWriteUps();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerWriteUps)

#endif // CMD_HANDLERS_QUESTS_H
