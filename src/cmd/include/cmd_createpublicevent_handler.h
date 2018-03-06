#ifndef CMD_CREATEPUBLICEVENT_HANDLER_H
#define CMD_CREATEPUBLICEVENT_HANDLER_H

#include <iserver.h>

class CmdCreatePublicEventHandler : public ICmdHandler {
	
	public:
		CmdCreatePublicEventHandler();
        virtual std::string cmd();
		virtual bool accessUnauthorized();
		virtual bool accessUser();
		virtual bool accessTester();
		virtual bool accessAdmin();
        virtual const std::vector<CmdInputDef> &inputs();
		virtual QString description();
		virtual QStringList errors();
        virtual void handle(ModelRequest *pRequest);
		
	private:
        QString TAG;
        std::vector<CmdInputDef> m_vInputs;
};

#endif // CMD_CREATEPUBLICEVENT_HANDLER_H
