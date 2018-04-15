#ifndef CMD_HANDLERS_QUESTS_H
#define CMD_HANDLERS_QUESTS_H

#include <cmd_handler_base.h>

/*********************************************
 * List of Quests
**********************************************/

class CmdHandlerQuests : public CmdHandlerBase {
	
	public:
        CmdHandlerQuests();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerQuests)

/*********************************************
 * Quest Info
**********************************************/

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

class CmdHandlerCreateQuest : public ICmdHandler {

    public:
        CmdHandlerCreateQuest();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerCreateQuest)

// *******************************************
// ************** Quest Delete ***************
// *******************************************

class CmdHandlerQuestDelete : public ICmdHandler {

    public:
        CmdHandlerQuestDelete();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerQuestDelete)

// *******************************************
// ************** Quest Proposal ***************
// *******************************************

class CmdHandlerQuestProposal : public ICmdHandler {

    public:
        CmdHandlerQuestProposal();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerQuestProposal)


// *******************************************
// ************** Quest Statistics ***********
// *******************************************

class CmdHandlerQuestStatistics : public ICmdHandler {

    public:
        CmdHandlerQuestStatistics();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerQuestStatistics)

// *******************************************
// ************** Quest Update ***********
// *******************************************

class CmdHandlerQuestUpdate : public ICmdHandler {

    public:
        CmdHandlerQuestUpdate();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerQuestUpdate)


// *******************************************
// ************** Quests Subjects ************
// *******************************************

class CmdHandlerQuestsSubjects : public ICmdHandler {

    public:
        CmdHandlerQuestsSubjects();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerQuestsSubjects)

// *******************************************
// ************ Quest Hints Add **************
// *******************************************

class CmdHandlerAddHint : public ICmdHandler {

    public:
        CmdHandlerAddHint();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;

};

REGISTRY_CMD(CmdHandlerAddHint)

// *******************************************
// ************ Quest AnswerList **************
// *******************************************

class CmdHandlerAnswerList : public ICmdHandler {

    public:
        CmdHandlerAnswerList();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerAnswerList)

// *******************************************
// *********** Quest Hints Delete ************
// *******************************************

class CmdHandlerDeleteHint : public ICmdHandler {

    public:
        CmdHandlerDeleteHint();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerDeleteHint)

// *******************************************
// **************** Quest Hints **************
// *******************************************

class CmdHandlerHints : public ICmdHandler {

    public:
        CmdHandlerHints();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerHints)

// *******************************************
// ********* Quests Proposal List ************
// *******************************************

class CmdHandlerQuestsProposalList : public ICmdHandler {

    public:
        CmdHandlerQuestsProposalList();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerQuestsProposalList)

// *******************************************
// ************ Quests WriteUps **************
// *******************************************

class CmdHandlerWriteUps : public ICmdHandler {

    public:
        CmdHandlerWriteUps();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdHandlerWriteUps)

#endif // CMD_HANDLERS_QUESTS_H
