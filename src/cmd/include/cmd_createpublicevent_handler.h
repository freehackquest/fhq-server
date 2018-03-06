#ifndef CMD_CREATEPUBLICEVENT_HANDLER_H
#define CMD_CREATEPUBLICEVENT_HANDLER_H

#include <iserver.h>

class CmdCreatePublicEventHandler : public ICmdHandler {
	
	public:
		CmdCreatePublicEventHandler();
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

#endif // CMD_CREATEPUBLICEVENT_HANDLER_H
