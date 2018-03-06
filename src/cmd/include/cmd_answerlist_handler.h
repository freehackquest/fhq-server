#ifndef CMD_ANSWERLIST_HANDLER_H
#define CMD_ANSWERLIST_HANDLER_H

#include <iserver.h>

class CmdAnswerListHandler : public ICmdHandler {
	
	public:
		CmdAnswerListHandler();
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

#endif // CMD_ANSWERLIST_HANDLER_H
