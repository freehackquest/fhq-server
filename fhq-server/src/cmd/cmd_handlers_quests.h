#ifndef CMD_HANDLERS_QUESTS_H
#define CMD_HANDLERS_QUESTS_H

#include <wsjcpp_jsonrpc20.h>

// *********************************************
// *************** List of Quests **************
// *********************************************

class CmdHandlerQuests : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuests();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuests)

// *********************************************
// *************** Quest Info ******************
// *********************************************

class CmdHandlerQuest : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuest();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);

};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuest)


// *******************************************
// *************** Quest Pass ****************
// *******************************************

class CmdHandlerQuestPass : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestPass();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuestPass)

// *******************************************
// ************** Quest Create ***************
// *******************************************

class CmdHandlerCreateQuest : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerCreateQuest();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerCreateQuest)

// *******************************************
// ************** Quest Delete ***************
// *******************************************

class CmdHandlerQuestDelete : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuestDelete)

// *******************************************
// ************** Quest Proposal ***************
// *******************************************

class CmdHandlerQuestProposal : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestProposal();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuestProposal)


// *******************************************
// ************** Quest Statistics ***********
// *******************************************

class CmdHandlerQuestStatistics : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestStatistics();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuestStatistics)

// *******************************************
// ************** Quest Update ***********
// *******************************************

class CmdHandlerQuestUpdate : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestUpdate();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuestUpdate)


// *******************************************
// ************** Quests Subjects ************
// *******************************************

class CmdHandlerQuestsSubjects : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestsSubjects();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuestsSubjects)

// *******************************************
// ************ Quest Hints Add **************
// *******************************************

class CmdHandlerAddHint : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerAddHint();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerAddHint)

// *******************************************
// ************ Quest AnswerList **************
// *******************************************

class CmdHandlerAnswerList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerAnswerList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerAnswerList)

// *******************************************
// *********** Quest Hints Delete ************
// *******************************************

class CmdHandlerDeleteHint : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerDeleteHint();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerDeleteHint)

// *******************************************
// **************** Quest Hints **************
// *******************************************

class CmdHandlerHints : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerHints();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerHints)

// *******************************************
// ********* Quests Proposal List ************
// *******************************************

class CmdHandlerQuestsProposalList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestsProposalList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuestsProposalList)

// *******************************************
// ********* Quests Proposal List ************
// *******************************************

class CmdHandlerQuestsFilesUpload : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerQuestsFilesUpload();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerQuestsFilesUpload)

#endif // CMD_HANDLERS_QUESTS_H
