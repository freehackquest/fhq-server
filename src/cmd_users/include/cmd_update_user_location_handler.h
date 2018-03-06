#ifndef CMD_UPDATE_USER_LOCATION_HANDLER_H
#define CMD_UPDATE_USER_LOCATION_HANDLER_H

#include <iserver.h>

class CmdUpdateUserLocationHandler : public ICmdHandler {
	
	public:
		CmdUpdateUserLocationHandler();
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
        std::vector<CmdInputDef> m_vInputs;
};

#endif // CMD_UPDATE_USER_LOCATION_HANDLER_H
